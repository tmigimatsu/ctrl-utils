import pathlib
import re
import shutil
import subprocess
import sys

import setuptools  # type: ignore
from setuptools.command import build_ext  # type: ignore
from setuptools.extern.packaging import version  # type: ignore


__version__ = "1.5.0"


class CMakeExtension(setuptools.Extension):
    def __init__(self, name):
        setuptools.Extension.__init__(self, name, sources=[])


class CMakeBuild(build_ext.build_ext):
    def run(self):
        if not self.inplace:
            try:
                out = subprocess.check_output(["cmake", "--version"])
            except OSError:
                raise RuntimeError(
                    "CMake must be installed to build the following extensions: "
                    + ", ".join(e.name for e in self.extensions)
                )

            m = re.search(r"version\s*([\d.]+)", out.decode())
            if m is None:
                raise RuntimeError("Could not find CMake version.")
            cmake_version = version.Version(m.group(1))
            if cmake_version < version.Version("3.13.0"):
                raise RuntimeError(
                    "CMake >= 3.13.0 is required. Install the latest CMake with 'pip install cmake'."
                )

        for extension in self.extensions:
            self.build_extension(extension)

    def build_extension(self, extension: setuptools.Extension):
        extension_dir = pathlib.Path(
            self.get_ext_fullpath(extension.name)
        ).parent.absolute()

        # Clean old build.
        for old_build in extension_dir.glob(
            "*.dylib" if sys.platform == "darwin" else "*.so"
        ):
            old_build.unlink()

        # Create new build folder.
        if self.inplace:
            build_dir = (pathlib.Path(__file__).parent / "build").absolute()
        else:
            build_dir = pathlib.Path(self.build_temp)
        build_dir.mkdir(parents=True, exist_ok=True)

        # Run CMake.
        build_type = "Debug" if self.debug else "Release"
        python_version = ".".join(map(str, sys.version_info[:3]))
        cmake_command = [
            "cmake",
            "-B" + str(build_dir),
            "-DBUILD_PYTHON=ON",
            "-DPYBIND11_PYTHON_VERSION=" + python_version,
            "-DCMAKE_BUILD_TYPE=" + build_type,
        ]
        if not self.inplace:
            # Use relative paths for install rpath.
            rpath_origin = "@loader_path" if sys.platform == "darwin" else "$ORIGIN"
            cmake_command += [
                "-DCMAKE_INSTALL_PREFIX=install",
                "-DCMAKE_INSTALL_RPATH=" + rpath_origin,
            ]
        print(*cmake_command)
        self.spawn(cmake_command)

        # Build and install.
        make_command = ["cmake", "--build", str(build_dir)]
        if not self.inplace:
            make_command += ["--target", "install"]

        ncpus = (
            subprocess.check_output(["./ncpu.sh"], cwd="cmake").strip().decode("utf-8")
        )
        make_command += ["--", "-j" + ncpus]

        self.spawn(make_command)

        if not self.inplace:
            # Copy pybind11 library.
            ctrlutils_dir = str(extension_dir / "ctrlutils")
            for file in (build_dir / "src" / "python").iterdir():
                if re.match(r".*\.(?:so|dylib)\.?", file.name) is not None:
                    shutil.move(str(file), ctrlutils_dir)

            # Copy C++ libraries.
            libdir = next(iter(pathlib.Path("install").glob("lib*")))
            for file in libdir.iterdir():
                if re.match(r".*\.(?:so|dylib)\.?", file.name) is not None:
                    shutil.move(str(file), ctrlutils_dir)


setuptools.setup(
    packages=["ctrlutils", "ctrlutils.eigen"],
    ext_modules=[CMakeExtension("ctrlutils")],
    cmdclass={
        "build_ext": CMakeBuild,
    },
)
