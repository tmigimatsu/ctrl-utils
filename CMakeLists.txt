############################################################
# CMakeLists for the control_utils library.
#
# Copyright 2018. All Rights Reserved.
#
# Created: May 7, 2018
# Authors: Toki Migimatsu
############################################################

# Require 3.11 to support setting INTERFACE_INCLUDE_DIRECTORIES on imported targets.
cmake_minimum_required(VERSION 3.11)

if(DEFINED PROJECT_NAME)
    set(IS_SUBPROJECT ON)
endif()

# Define project
project(ctrl_utils VERSION 1.3.0 LANGUAGES CXX)

# Define options
option(CTRL_UTILS_BUILD_PYTHON "Build Python library" OFF)

# Set CMake flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Define directories
set(CTRL_UTILS_LIB ctrl_utils)
set(LIB_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
set(LIB_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(LIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(LIB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(ctrl_utils_EXTERNAL_DIR ${LIB_EXTERNAL_DIR} CACHE INTERNAL "")

# Create control_utils library
if(NOT TARGET ${CTRL_UTILS_LIB})
add_library(${CTRL_UTILS_LIB} INTERFACE)

# Create namespaced alias for use inside the build tree
add_library(${CTRL_UTILS_LIB}::${CTRL_UTILS_LIB} ALIAS ${CTRL_UTILS_LIB})

# Find dependencies
include(${LIB_CMAKE_DIR}/include_package.cmake)

# Set include directories
target_include_directories(${CTRL_UTILS_LIB}
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${LIB_INCLUDE_DIR}>
)

# Use GNUInstalDirs to install ibraries into correct locations on all platforms
include(GNUInstallDirs)

set(CTRL_UTILS_TARGETS ${CTRL_UTILS_LIB}Targets)
set(CTRL_UTILS_CONFIG ${CTRL_UTILS_LIB}Config)
set(CTRL_UTILS_CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${CTRL_UTILS_LIB})

# Install the library
install(TARGETS ${CTRL_UTILS_LIB}
    EXPORT ${CTRL_UTILS_TARGETS}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install the headers
install(DIRECTORY ${LIB_INCLUDE_DIR}/${CTRL_UTILS_LIB} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export targets to Targets.cmake
install(EXPORT ${CTRL_UTILS_TARGETS}
    DESTINATION ${CTRL_UTILS_CONFIG_INSTALL_DIR}
    NAMESPACE ${CTRL_UTILS_LIB}::
    FILE ${CTRL_UTILS_TARGETS}.cmake
)

# Create ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}Version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Create Config.cmake
configure_package_config_file(${LIB_CMAKE_DIR}/${CTRL_UTILS_CONFIG}.cmake.in
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}.cmake
    INSTALL_DESTINATION ${CTRL_UTILS_CONFIG_INSTALL_DIR})

# Install config script files
install(FILES
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}.cmake
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}Version.cmake
    DESTINATION ${CTRL_UTILS_CONFIG_INSTALL_DIR}
)

# Build Python wrapper
if(CTRL_UTILS_BUILD_PYTHON)
    add_subdirectory(${LIB_SOURCE_DIR}/python)
endif()

export(TARGETS ${CTRL_UTILS_LIB}
    NAMESPACE ${CTRL_UTILS_LIB}::
    FILE ${CMAKE_BINARY_DIR}/${CTRL_UTILS_TARGETS}.cmake)

# Register package in user registry
export(PACKAGE ${CTRL_UTILS_LIB})
endif()

