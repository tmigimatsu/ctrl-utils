############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET pybind11::pybind11)
    init_git_submodule(pybind11.git)
    add_subdirectory(pybind11.git)
endif()
