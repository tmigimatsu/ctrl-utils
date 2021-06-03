############################################################
# CMakeLists for Pybind11
#
# Copyright 2019. All Rights Reserved.
#
# Created: October 30, 2019
# Authors: Toki Migimatsu
############################################################

project(ctrlutils)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIB_SOURCE_DIR}/ctrlutils")

ctrl_utils_add_subdirectory(pybind11)
ctrl_utils_add_subdirectory(Eigen3)

# Create ctrl-utils wrapper.
pybind11_add_module(ctrlutils ctrl_utils.cc eigen.cc)

# Link library dependencies.
target_link_libraries(ctrlutils
  PUBLIC
    Eigen3::Eigen
    pybind11::pybind11
    ctrl_utils::ctrl_utils
)

# Create Eigen wrapper.
pybind11_add_module(ctrlutilseigen eigen.cc)

# Link library dependencies.
target_link_libraries(ctrlutilseigen
  PUBLIC
    Eigen3::Eigen
    pybind11::pybind11
    ctrl_utils::ctrl_utils
)

# Put binary for installation temporarily in build/src/python.
install(TARGETS ctrlutils
    LIBRARY DESTINATION "${PROJECT_BINARY_DIR}"
)
install(TARGETS ctrlutilseigen
    LIBRARY DESTINATION "${PROJECT_BINARY_DIR}"
)
