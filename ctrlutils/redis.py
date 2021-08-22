"""
redis.py

Copyright 2021. All Rights Reserved.

Created: May 23, 2021
Authors: Toki Migimatsu
"""

import typing

import redis
import numpy as np


class StringStream:
    def __init__(self, buffer: bytes):
        self._buffer = buffer
        self._idx = 0

    def getbuffer(self) -> bytes:
        return self._buffer[self._idx :]

    def read(self, num_bytes: int) -> bytes:
        idx_prev = self._idx
        self._idx += num_bytes
        return self._buffer[idx_prev : self._idx]

    def read_word(self) -> str:
        len_word = self.getbuffer().index(b" ")
        word = self.read(len_word)
        self.read(1)  # Consume space.
        return word.decode("utf8")


def decode_matlab(s: typing.Union[str, bytes]) -> np.ndarray:
    if isinstance(s, bytes):
        s = s.decode("utf8")
    s = s.strip()
    tokens = [list(map(float, row.strip().split())) for row in s.split(";")]
    return np.array(tokens).squeeze()


def encode_matlab(A: np.ndarray) -> str:
    if len(A.shape) == 1:
        return " ".join(map(str, A.tolist()))
    return "; ".join(" ".join(map(str, row)) for row in A.tolist())


def decode_opencv(b: bytes) -> np.ndarray:
    import cv2

    ss = StringStream(b)

    mat_type = int(ss.read_word())
    if mat_type in {
        cv2.CV_8UC1,
        cv2.CV_8UC2,
        cv2.CV_8UC3,
        cv2.CV_8UC4,
        cv2.CV_16UC1,
        cv2.CV_16UC2,
        cv2.CV_16UC3,
        cv2.CV_16UC4,
        cv2.CV_32FC1,
        cv2.CV_32FC2,
        cv2.CV_32FC3,
        cv2.CV_32FC4,
    }:
        size = int(ss.read_word())
        buffer = np.frombuffer(ss.getbuffer(), dtype=np.uint8)
        img = cv2.imdecode(buffer, cv2.IMREAD_UNCHANGED)
    else:
        rows = int(ss.read_word())
        cols = int(ss.read_word())
        buffer = np.frombuffer(ss.getbuffer(), dtype=np.uint8)
        img = buffer.reshape((rows, cols))

    return img

def encode_opencv(img: np.ndarray) -> bytes:
    import cv2
    def np_to_cv_type(img: np.ndarray):
        if img.dtype == np.uint8:
            if len(img.shape) == 2 or img.shape[2] == 1:
                return cv2.CV_8UC1
            elif img.shape[2] == 2:
                return cv2.CV_8UC2
            elif img.shape[2] == 3:
                return cv2.CV_8UC3
            elif img.shape[2] == 4:
                return cv2.CV_8UC4
        elif img.dtype == np.uint16:
            if len(img.shape) == 2 or img.shape[2] == 1:
                return cv2.CV_16UC1
            elif img.shape[2] == 2:
                return cv2.CV_16UC2
            elif img.shape[2] == 3:
                return cv2.CV_16UC3
            elif img.shape[2] == 4:
                return cv2.CV_16UC4
        elif img.dtype == np.float32:
            if len(img.shape) == 2 or img.shape[2] == 1:
                return cv2.CV_32FC1
            elif img.shape[2] == 2:
                return cv2.CV_32FC2
            elif img.shape[2] == 3:
                return cv2.CV_32FC3
            elif img.shape[2] == 4:
                return cv2.CV_32FC4
        raise ArgumentError("Unsupported image type {img.dtype}, {img.shape[2]} channels")

    buffer = []
    type_img = np_to_cv_type(img)
    buffer.append(f"{type_img} ".encode("utf8"))

    if img.dtype in (np.uint8, np.uint16):
        _, png = cv2.imencode(".png", img)
        buffer.append(f"{len(png)} ".encode("utf8"))
        buffer.append(png.tobytes())
    elif img.dtype == np.float32:
        _, exr = cv2.imencode(".exr", img)
        buffer.append(f"{len(exr)} ".encode("utf8"))
        buffer.append(exr.tobytes())
    else:
        buffer.append(f"{img.shape[0]} {img.shape[1]} ".encode("utf8"))
        buffer.append(img.tobytes())

    return b"".join(buffer)


class RedisClient(redis.Redis):
    def __init__(
        self,
        host: str = "127.0.0.1",
        port: int = 6379,
        password: typing.Optional[str] = None,
    ):
        super().__init__(host=host, port=port, password=password)

    def pipeline(self, transaction=True, shard_hint=None):
        return Pipeline(
            self.connection_pool, self.response_callbacks, transaction, shard_hint
        )

    def get_image(self, key: str) -> np.ndarray:
        """Gets a cv::Mat image from Redis."""
        val = self.get(key)
        return decode_opencv(val)

    def set_image(self, key: str, val: np.ndarray):
        """Sets a cv::Mat in Redis."""
        self.set(key, encode_opencv(val))

    def get_matrix(self, key: str) -> np.ndarray:
        """Gets an Eigen::Matrix or Eigen::Vector from Redis."""
        val = self.get(key).decode("utf8")
        return decode_matlab(val)

    def set_matrix(self, key: str, val: np.ndarray):
        """Sets an Eigen::Matrix or Eigen::Vector in Redis."""
        self.set(key, encode_matlab(val))


class Pipeline(redis.client.Pipeline):
    def __init__(self, connection_pool, response_callbacks, transaction, shard_hint):
        super().__init__(connection_pool, response_callbacks, transaction, shard_hint)

    def set_image(self, key: str, val: np.ndarray):
        """Sets a cv::Mat in Redis."""
        self.set(key, encode_opencv(val))

    def set_matrix(self, key: str, val: np.ndarray):
        """Sets an Eigen::Matrix or Eigen::Vector in Redis."""
        self.set(key, encode_matlab(val))
