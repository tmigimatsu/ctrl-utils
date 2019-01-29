############################################################
# CMakeLists for the control_utils library.
#
# Copyright 2018. All Rights Reserved.
#
# Created: May 7, 2018
# Authors: Toki Migimatsu
############################################################

# Require 3.6 to support pkg_check_modules IMPORTED_TARGET
cmake_minimum_required(VERSION 3.6)

# Define project
project(ctrl_utils VERSION 1.0.0 LANGUAGES CXX)

# Set CMake flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# Define directories
set(CTRL_UTILS_LIB ctrl_utils)
set(CTRL_UTILS_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
set(CTRL_UTILS_EXT_DIR ${PROJECT_SOURCE_DIR}/external)
set(CTRL_UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Create control_utils library
add_library(${CTRL_UTILS_LIB} INTERFACE)

# Create namespaced alias for use inside the build tree
add_library(${CTRL_UTILS_LIB}::${CTRL_UTILS_LIB} ALIAS ${CTRL_UTILS_LIB})

# Find dependencies
function(download_package PACKAGE_NAME)
    configure_file(${CTRL_UTILS_CMAKE_DIR}/${PACKAGE_NAME}-CMakeLists.txt.in
        ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_NAME})
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_NAME})
endfunction()

# Export for build tree
configure_file(${CTRL_UTILS_CMAKE_DIR}/Findcpp_redis.cmake.in
    ${CMAKE_BINARY_DIR}/Findcpp_redis.cmake @ONLY)
configure_file(${CTRL_UTILS_CMAKE_DIR}/FindEigen3.cmake.in
    ${CMAKE_BINARY_DIR}/FindEigen3.cmake @ONLY)
configure_file(${CTRL_UTILS_CMAKE_DIR}/Findnlohmann_json.cmake.in
    ${CMAKE_BINARY_DIR}/Findnlohmann_json.cmake @ONLY)
configure_file(${CTRL_UTILS_CMAKE_DIR}/Findyaml-cpp.cmake.in
    ${CMAKE_BINARY_DIR}/Findyaml-cpp.cmake @ONLY)

find_package(cpp_redis 4.3.1 QUIET)
if(NOT cpp_redis_FOUND)
    download_package(cpp_redis)
    list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_BINARY_DIR})
    find_package(cpp_redis 4.3.1 MODULE REQUIRED)
    list(REMOVE_AT CMAKE_MODULE_PATH 0)
endif()

find_package(Eigen3 3.3.5 QUIET)
if(NOT Eigen3_FOUND)
    download_package(Eigen3)
    find_package(Eigen3 3.3.5 REQUIRED PATHS ${CMAKE_BINARY_DIR}/Eigen3/build)
endif()

find_package(nlohmann_json 3.2 QUIET)
if(NOT nlohmann_json_FOUND)
    download_package(nlohmann_json)
    find_package(nlohmann_json 3.2 REQUIRED PATHS ${CMAKE_BINARY_DIR}/nlohmann_json/build)
endif()

# find_package(yaml-cpp 0.6.2 QUIET)
if(NOT yaml-cpp_FOUND)
    download_package(yaml-cpp)
    list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_BINARY_DIR})
    find_package(yaml-cpp 0.6.2 REQUIRED)
    list(REMOVE_AT CMAKE_MODULE_PATH 0)
else()
    add_library(yaml-cpp::yaml-cpp INTERFACE IMPORTED)
    set_target_properties(yaml-cpp::yaml-cpp PROPERTIES
        INTERFACE_LINK_LIBRARIES yaml-cpp
        INTERFACE_INCLUDE_DIRECTORIES ${YAML_CPP_INCLUDE_DIR})
endif()

target_link_libraries(${CTRL_UTILS_LIB}
    INTERFACE
        cpp_redis::cpp_redis
        Eigen3::Eigen
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
)

# Set include directories
target_include_directories(${CTRL_UTILS_LIB}
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CTRL_UTILS_INCLUDE_DIR}>
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
install(DIRECTORY ${CTRL_UTILS_INCLUDE_DIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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
    COMPATIBILITY AnyNewerVersion
)

# Create Config.cmake
configure_package_config_file(${CTRL_UTILS_CMAKE_DIR}/${CTRL_UTILS_CONFIG}.cmake.in
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}.cmake
    INSTALL_DESTINATION ${CTRL_UTILS_CONFIG_INSTALL_DIR})

# Install config script files
install(FILES
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}.cmake
    ${CMAKE_BINARY_DIR}/${CTRL_UTILS_CONFIG}Version.cmake
    DESTINATION ${CTRL_UTILS_CONFIG_INSTALL_DIR}
)

export(TARGETS ${CTRL_UTILS_LIB}
    NAMESPACE ${CTRL_UTILS_LIB}::
    FILE ${CMAKE_BINARY_DIR}/${CTRL_UTILS_TARGETS}.cmake)

# Register package in user registry
export(PACKAGE ${CTRL_UTILS_LIB})

