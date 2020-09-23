############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

init_git_submodule(pybind11.git)
add_subdirectory(pybind11.git ${CMAKE_BINARY_DIR}/pybind11)
