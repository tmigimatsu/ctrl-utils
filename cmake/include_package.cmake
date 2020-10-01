############################################################
# Utility scripts for downloading external packages.
#
# Copyright 2020. All Rights Reserved.
#
# Created: June 5, 2020
# Authors: Toki Migimatsu
############################################################

function(ctrl_utils_add_subdirectory SUBDIRECTORY)
    set(EXTERNAL_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/external)
    add_subdirectory(${ctrl_utils_EXTERNAL_DIR}/${SUBDIRECTORY} ${EXTERNAL_BINARY_DIR}/${SUBDIRECTORY})
endfunction()
