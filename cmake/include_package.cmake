############################################################
# Utility scripts for downloading external packages
#
# Copyright 2020. All Rights Reserved.
#
# Created: June 5, 2020
# Authors: Toki Migimatsu
############################################################

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_BINARY_DIR} ${LIB_CMAKE_DIR})

function(include_package PACKAGE_NAME)
    # Set optional package version
    set(EXT_PACKAGE_VERSION ${ARGV1})

    # See if package already exists
    find_package(${PACKAGE_NAME} ${EXT_PACKAGE_VERSION} QUIET)
    if(${PACKAGE_NAME}_FOUND)
        return()
    endif()

    if(EXISTS ${LIB_EXTERNAL_DIR}/${PACKAGE_NAME}.git)
        include_git_submodule(${PACKAGE_NAME}.git)
    endif()

    # # Configure custom FindPackage.cmake script if exists
    # if(EXISTS ${LIB_CMAKE_DIR}/Find${PACKAGE_NAME}.cmake.in)
    #     configure_file(${LIB_CMAKE_DIR}/Find${PACKAGE_NAME}.cmake.in
    #         ${CMAKE_BINARY_DIR}/Find${PACKAGE_NAME}.cmake @ONLY)
    # endif()

    # # Configure custom package-CMakeLists.txt script
    # configure_file(${LIB_CMAKE_DIR}/${PACKAGE_NAME}-CMakeLists.txt.in
    #     ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}/CMakeLists.txt @ONLY)

    # # Run CMake
    # execute_process(COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
    #     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_NAME})
    # execute_process(COMMAND ${CMAKE_COMMAND} --build .
    #     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_NAME})

    # # Find package for last time
    # find_package(${PACKAGE_NAME} ${EXT_PACKAGE_VERSION} REQUIRED)
endfunction()

function(include_git_submodule GIT_REPO)
    find_package(Git REQUIRED)

    # Update submodule
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${GIT_REPO}
        WORKING_DIRECTORY "${LIB_EXTERNAL_DIR}"
        RESULT_VARIABLE git_submodule_result
    )
    if(NOT git_submodule_result EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_REPO}")
    endif()

    add_subdirectory(${LIB_EXTERNAL_DIR}/${PACKAGE_NAME}.git)
endfunction()

function(init_git_submodule GIT_SUBMODULE)
    # Update submodule
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive ${GIT_SUBMODULE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE git_submodule_result
    )
    if(NOT git_submodule_result EQUAL "0")
        message(FATAL_ERROR "${GIT_EXECUTABLE} submodule update --init --recursive ${GIT_SUBMODULE} failed with error:\n ${git_submodule_result}")
    endif()
endfunction()
