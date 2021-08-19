############################################################
# CMakeLists for Pybind11
#
# Copyright 2019. All Rights Reserved.
#
# Created: October 30, 2019
# Authors: Toki Migimatsu
############################################################

project(ctrlutils)

message(STATUS "Configuring Python wrapper for ${PROJECT_NAME}")

set(PYTHON_LIB_NAME ctrlutils)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_SOURCE_DIR}/${PYTHON_LIB_NAME}")

# Create the library.
ctrl_utils_add_subdirectory(pybind11)
pybind11_add_module(${PYTHON_LIB_NAME} ctrl_utils.cc eigen.cc)

# Link library dependencies.
ctrl_utils_add_subdirectory(Eigen3)
target_link_libraries(${PYTHON_LIB_NAME}
  PUBLIC
    ctrl_utils::ctrl_utils
    Eigen3::Eigen
    pybind11::pybind11
)

# Create Eigen wrapper.
pybind11_add_module(${PYTHON_LIB_NAME}_eigen eigen.cc)

# Link library dependencies.
target_link_libraries(${PYTHON_LIB_NAME}_eigen
  PUBLIC
    ctrl_utils::ctrl_utils
    Eigen3::Eigen
    pybind11::pybind11
)

# Put binary for installation temporarily in build/src/python.
install(TARGETS ${PYTHON_LIB_NAME}
    LIBRARY DESTINATION "${PROJECT_BINARY_DIR}"
)
install(TARGETS ${PYTHON_LIB_NAME}_eigen
    LIBRARY DESTINATION "${PROJECT_BINARY_DIR}"
)
