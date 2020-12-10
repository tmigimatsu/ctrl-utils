############################################################
# Utility scripts for downloading external packages.
#
# Copyright 2020. All Rights Reserved.
#
# Created: June 5, 2020
# Authors: Toki Migimatsu
############################################################

##########
# Add a ctrl_utils dependency as a subdirectory.
#
# Usage:
#     ctrl_utils_add_subdirectory(cpp_redis)
#     ctrl_utils_add_subdirectory(cpp_redis INCLUDE_ALL_TARGETS)
function(ctrl_utils_add_subdirectory subdirectory)
    set(INCLUDE_ALL_TARGETS "EXCLUDE_FROM_ALL")
    if((ARGC GREATER 1) AND (ARGV1 STREQUAL "INCLUDE_ALL_TARGETS"))
        set(INCLUDE_ALL_TARGETS "")
    endif()

    set(CTRL_UTILS_EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/external")
    set(EXTERNAL_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/external")

    add_subdirectory("${CTRL_UTILS_EXTERNAL_DIR}/${subdirectory}"
        "${EXTERNAL_BINARY_DIR}/${subdirectory}"
        ${INCLUDE_ALL_TARGETS}
    )
endfunction()
