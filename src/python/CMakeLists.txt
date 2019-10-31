############################################################
# CMakeLists for Pybind11
#
# Copyright 2019. All Rights Reserved.
#
# Created: October 30, 2019
# Authors: Toki Migimatsu
############################################################

#############
# Pybind11 #
#############

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/ctrlutils)

set(PYBIND11_LIB_DIR ${LIB_EXTERNAL_DIR}/pybind11.git)
add_subdirectory(${PYBIND11_LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/pybind11)

pybind11_add_module(ctrlutils ctrl_utils.cc)
target_link_libraries(ctrlutils PUBLIC pybind11::pybind11 ctrl_utils::ctrl_utils)

