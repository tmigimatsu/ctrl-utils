[![Builds](https://github.com/tmigimatsu/ctrl-utils/actions/workflows/builds.yaml/badge.svg)](https://github.com/tmigimatsu/ctrl-utils/actions/workflows/builds.yaml)
[![Docs](https://github.com/tmigimatsu/ctrl-utils/actions/workflows/docs.yaml/badge.svg)](https://github.com/tmigimatsu/ctrl-utils/actions/workflows/docs.yaml)

# ctrl-utils

`ctrl-utils` is a C++/Python utilities library for robot control.

See the documentation for `ctrl-utils`
[here](https://tmigimatsu.github.io/ctrl-utils/).

## Installation Requirements

This library is written in C++ with Python bindings automatically generated with
[pybind11](https://github.com/pybind/pybind11). It has been tested on
Ubuntu 18.04, Ubuntu 20.04, and macOS 10.15 Catalina.

Compilation requirements:
- `cmake >= 3.11`
- C++17 support (`gcc >= 7`, `clang >= 7`).

Ubuntu 20.04 meets these requirements out of the box. Instructions for other
OSes are below:

### Ubuntu 18.04

The simplest way to install the latest version of `cmake` is through `pip`:
```sh
pip install cmake
```

You can also install it through `apt`:
```sh
sudo apt-get update && sudo apt-get install -y apt-transport-https ca-certificates gnupg wget
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository -y 'deb https://apt.kitware.com/ubuntu/ bionic main'
sudo apt-get update && sudo apt-get install -y cmake kitware-archive-keyring
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
```

### macOS

Install `cmake` through Homebrew:
```sh
brew install cmake
```

Or through `pip`:
```sh
pip3 install cmake
```

## Installation

First, clone the repository:
```sh
git clone https://github.com/tmigimatsu/ctrl-utils.git
cd ctrl-utils
```

### C++ only

The C++ portion of `ctrl-utils` is header-only, but to add `ctrl-utils` as a
`cmake` dependency, you can run the following:
```sh
mkdir build
cmake -B build
```

### Python only

Use `pip` to install `ctrl-utils` in your virtual environment.
```sh
pip install .
```

You can now import the `ctrlutils` package in Python.
```py
import ctrlutils
```

### C++ and Python

An in-place `pip` install will run the appropriate CMake command to build
`ctrl-utils` locally in the `./build` folder. This will give you access to the
`cmake` configuration files for C++ as well as the `ctrlutils` package in
Python.
```sh
pip install -e .
```
