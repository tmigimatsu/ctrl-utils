############################################################
# CMakeLists for Pybind11.
#
# Copyright 2019. All Rights Reserved.
#
# Created: October 30, 2019
# Authors: Toki Migimatsu
############################################################

message(STATUS "Configuring Python wrapper for ${PROJECT_NAME}")

set(PYTHON_LIB_NAME ctrlutils)
set(PYTHON_LIB_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${PYTHON_LIB_NAME}")

# Create the library.
ctrl_utils_add_subdirectory(pybind11)
pybind11_add_module(${PYTHON_LIB_NAME} ctrl_utils.cc)

set_target_properties(${PYTHON_LIB_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PYTHON_LIB_DIR}"
)

# Link library dependencies.
ctrl_utils_add_subdirectory(Eigen3)
target_link_libraries(${PYTHON_LIB_NAME}
  PUBLIC
    ctrl_utils::ctrl_utils
    Eigen3::Eigen
    pybind11::pybind11
)

# Install python scripts.
set(PYTHON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/__init__.py
    ${CMAKE_CURRENT_SOURCE_DIR}/numpy.py
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.py
)
add_custom_command(
    TARGET ${PYTHON_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PYTHON_SOURCES} "${PYTHON_LIB_DIR}"
)
