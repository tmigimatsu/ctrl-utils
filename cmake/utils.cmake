############################################################
# Utility scripts for downloading external packages
#
# Copyright 2020. All Rights Reserved.
#
# Created: June 5, 2020
# Authors: Toki Migimatsu
############################################################

##########
# Initialize git submodule.
#
# Usage:
#     init_git_submodule(ctrl-utils.git)
#     init_git_submodule(ctrl-utils.git RECURSIVE)
function(init_git_submodule GIT_SUBMODULE)
    set(RECURSIVE "")
    if(DEFINED ARGV1)
        if (${ARGV1})
            set(RECURSIVE "--recursive")
        endif()
    endif()

    # Update submodule
    find_package(Git REQUIRED)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${GIT_SUBMODULE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE git_submodule_result
    )
    if(NOT git_submodule_result EQUAL "0")
        message(FATAL_ERROR "${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${GIT_SUBMODULE} failed with error:\n ${git_submodule_result}")
    endif()
endfunction()

##########
# Add an external dependency as a subdirectory.
#
# Usage:
#     lib_add_subdirectory(ctrl_utils)
#     lib_add_subdirectory(ctrl_utils INCLUDE_ALL_TARGETS)
function(ctrl_utils_add_subdirectory SUBDIRECTORY)
    set(EXTERNAL_BINARY_DIR ${PROJECT_BINARY_DIR}/external)
    add_subdirectory(${ctrl_utils_EXTERNAL_DIR}/${SUBDIRECTORY} ${EXTERNAL_BINARY_DIR}/${SUBDIRECTORY})
endfunction()

##########
# Set an option to be true by default if the condition is true.
#
# Usage:
#     lib_dependent_option(MAIN_BUILD "Main build." "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME")
function(lib_dependent_option name description condition)
    string(REGEX REPLACE " +" ";" OPTION_CONDITION "${condition}")
    if(${OPTION_CONDITION})
        option(${name} ${description} ON)
    else()
        option(${name} ${description} OFF)
    endif()
endfunction()

##########
# Create two version of an option:
# - OPTION set to the given default value if MAIN_BUILD is true.
# - LIBNAME_OPTION set to true if MAIN_BUILD and OPTION are true, otherwise
# false.
#
# Usage:
#     lib_option(BUILD_EXAMPLES "Build examples." ON)
function(lib_option name description default)
    if(${LIB_CMAKE_NAME}_MAIN_BUILD)
        option(${name} ${description} ${default})
    endif()
    lib_dependent_option(${LIB_CMAKE_NAME}_${name} ${description} "${LIB_CMAKE_NAME}_MAIN_BUILD AND ${name}")
endfunction()
