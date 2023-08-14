"""
redis.py

Copyright 2021. All Rights Reserved.

Created: May 23, 2021
Authors: Toki Migimatsu
"""

from typing import Optional, Union

import numpy as np
import redis


class InputStringStream:
    def __init__(self, buffer: bytes):
        self._buffer = buffer
        self._idx = 0

    def peek_remaining(self) -> bytes:
        return self._buffer[self._idx :]

    def read(self, num_bytes: int) -> bytes:
        idx_prev = self._idx
        self._idx += num_bytes
        return self._buffer[idx_prev : self._idx]

    def read_word(self) -> str:
        len_word = self.peek_remaining().index(b" ")
        word = self.read(len_word)
        self.read(1)  # Consume space.
        return word.decode("utf8")


class OutputStringStream:
    def __init__(self, buffer: Optional[list[bytes]] = None) -> None:
        self._buffer = [] if buffer is None else buffer

    def write(self, b: Union[bytes, str]) -> None:
        if isinstance(b, str):
            b = b.encode("utf8")
        self._buffer.append(b)

    def flush(self) -> bytes:
        self._buffer = [b"".join(self._buffer)]
        return self._buffer[0]


def decode_matlab(s: Union[str, bytes]) -> np.ndarray:
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

    ss = InputStringStream(b)

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
        buffer = np.frombuffer(ss.peek_remaining(), dtype=np.uint8)
        img = cv2.imdecode(buffer, cv2.IMREAD_UNCHANGED)
    else:
        raise ValueError(f"Unsupported image type {mat_type}.")

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
        raise ValueError(
            f"Unsupported image type {img.dtype}, {img.shape[2] if len(img.shape) > 2 else 1} channels"
        )

    type_img = np_to_cv_type(img)

    if img.dtype in (np.uint8, np.uint16):
        _, data = cv2.imencode(".png", img)
    elif img.dtype == np.float32:
        _, data = cv2.imencode(".exr", img)

    ss = OutputStringStream()
    ss.write(f"{type_img} {len(data)} ")
    ss.write(data.tobytes())

    return ss.flush()


def decode_tensor(b: bytes) -> np.ndarray:
    ss = InputStringStream(b)

    # Parse shape opening delimiter.
    w = ss.read_word()
    if w != "(":
        raise ValueError(f"Expected '(' at index 0 but found {w} instead.")

    # Parse shape.
    shape = []
    while True:
        w = ss.read_word()
        if w == ")":
            break
        shape.append(int(w))

    # Parse dtype
    dtype = np.dtype(ss.read_word())

    # Parse data.
    if dtype == "bool":
        tensor = np.frombuffer(ss.peek_remaining(), dtype=np.uint8)
        tensor = np.unpackbits(tensor, count=np.prod(shape)).astype(bool)
    else:
        tensor = np.frombuffer(ss.peek_remaining(), dtype=dtype)
    tensor = tensor.reshape(shape)

    return tensor


def encode_tensor(tensor: np.ndarray) -> bytes:
    ss = OutputStringStream()
    shape = " ".join(map(str, tensor.shape))
    dtype = str(tensor.dtype)
    ss.write(f"( {shape} ) {dtype} ")
    if dtype == "bool":
        tensor = np.packbits(tensor)
    ss.write(tensor.tobytes())
    return ss.flush()


class RedisClient(redis.Redis):
    def __init__(
        self,
        host: str = "127.0.0.1",
        port: int = 6379,
        password: Optional[str] = None,
    ) -> None:
        super().__init__(host=host, port=port, password=password)

    def pipeline(self, transaction: bool = True, shard_hint=None) -> "Pipeline":
        return Pipeline(
            self.connection_pool, self.response_callbacks, transaction, shard_hint
        )

    def get(self, key: str, decode: Optional[str] = None) -> str:
        val = super().get(key)
        if decode is not None:
            return val.decode("utf8")
        return val

    def get_image(self, key: str) -> np.ndarray:
        """Gets a cv::Mat image from Redis."""
        b_val = super().get(key)
        return decode_opencv(b_val)

    def set_image(self, key: str, val: np.ndarray) -> bool:
        """Sets a cv::Mat in Redis."""
        return self.set(key, encode_opencv(val))

    def get_matrix(self, key: str) -> np.ndarray:
        """Gets an Eigen::Matrix or Eigen::Vector from Redis."""
        b_val = self.get(key)
        return decode_matlab(b_val)

    def set_matrix(self, key: str, val: np.ndarray) -> bool:
        """Sets an Eigen::Matrix or Eigen::Vector in Redis."""
        return self.set(key, encode_matlab(val))

    def get_tensor(self, key: str) -> np.ndarray:
        """Gets a np.ndarray from Redis."""
        b_val = super().get(key)
        return decode_tensor(b_val)

    def set_tensor(self, key: str, val: np.ndarray) -> bool:
        """Sets a np.ndarray in Redis."""
        return self.set(key, encode_tensor(val))


class Pipeline(redis.client.Pipeline):
    def __init__(self, connection_pool, response_callbacks, transaction, shard_hint):
        super().__init__(connection_pool, response_callbacks, transaction, shard_hint)
        self._decode_fns = []

    def get(self, key: str, decode: Optional[str] = None) -> "Pipeline":
        super().get(key)
        self._decode_fns.append(None if decode is None else lambda b: b.decode(decode))
        return self

    def set(self, key: str, val) -> "Pipeline":
        super().set(key, val)
        self._decode_fns.append(None)
        return self

    def get_image(self, key: str) -> "Pipeline":
        """Gets a cv::Mat from Redis."""
        super().get(key)
        self._decode_fns.append(decode_opencv)
        return self

    def set_image(self, key: str, val: np.ndarray) -> "Pipeline":
        """Sets a cv::Mat in Redis."""
        return self.set(key, encode_opencv(val))

    def get_matrix(self, key: str) -> "Pipeline":
        """Gets an Eigen::Matrix or Eigen::Vector from Redis."""
        super().get(key)
        self._decode_fns.append(decode_matlab)
        return self

    def set_matrix(self, key: str, val: np.ndarray) -> "Pipeline":
        """Sets an Eigen::Matrix or Eigen::Vector in Redis."""
        return self.set(key, encode_matlab(val))

    def get_tensor(self, key: str) -> "Pipeline":
        """Gets a tensor from Redis."""
        super().get(key)
        self._decode_fns.append(decode_tensor)
        return self

    def set_tensor(self, key: str, val: np.ndarray) -> "Pipeline":
        """Sets a tensor in Redis."""
        return self.set(key, encode_tensor(val))

    def execute(self) -> list:
        responses = super().execute()
        decoded_responses = [
            decode_fn(response) if decode_fn is not None else response
            for response, decode_fn in zip(responses, self._decode_fns)
        ]
        self._decode_fns = []
        return decoded_responses
