############################################################
# CMakeLists for the ctrl_utils library.
#
# Copyright 2018. All Rights Reserved.
#
# Created: May 7, 2018
# Authors: Toki Migimatsu
############################################################

# Require 3.11 to support setting INTERFACE_INCLUDE_DIRECTORIES on imported targets.
cmake_minimum_required(VERSION 3.11)

# Define project
project(ctrl_utils
    VERSION 1.5.1
    DESCRIPTION "Utility library for robot control"
    LANGUAGES CXX
)
string(TOUPPER ${PROJECT_NAME} LIB_CMAKE_NAME)

# Set directory variables.
set(LIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIB_EXTERNAL_DIR "${LIB_SOURCE_DIR}/external")
set(CTRL_UTILS_EXTERNAL_DIR ${LIB_EXTERNAL_DIR} CACHE INTERNAL "")

# Include custom CMake utilities.
include(cmake/ctrl_utils_add_subdirectory.cmake)
include(cmake/utils.cmake)

# Detect whether this project is built by itself or included as a subdirectory.
lib_dependent_option(${LIB_CMAKE_NAME}_MAIN_BUILD
    "Build ${PROJECT_NAME} as a main project."
    "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME"
)

# Generate a compilation database for autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define CMake options
lib_option(BUILD_DOCS "Build docs." OFF)
lib_option(BUILD_PYTHON "Build Python library" OFF)
lib_option(CLANG_TIDY "Perform clang-tidy checks." OFF)

# Set default build type to release.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set output directories.
if(${LIB_CMAKE_NAME}_MAIN_BUILD)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
endif()

# Standardize install directories.
include(GNUInstallDirs)

# Build the library.
add_subdirectory(src)

# Build docs.
if(${LIB_CMAKE_NAME}_BUILD_DOCS)
    find_package(Doxygen)
    if(NOT Doxygen_FOUND)
        message(WARNING "Unable to find Doxygen for ${PROJECT_NAME} docs.")
    else()
        add_subdirectory(docs)
    endif()
endif()
