############################################################
# CMakeLists for ctrl-utils docs.
#
# Copyright 2018. All Rights Reserved.
#
# Created: September 11, 2018
# Authors: Toki Migimatsu
############################################################

message(STATUS "Configuring ${PROJECT_NAME} documentation.")

add_custom_target(ctrlutils_docs ALL
    COMMAND pipenv run make -C docs clean && pipenv run make -C docs
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

ctrl_utils_add_subdirectory(cpp_redis)
add_dependencies(ctrlutils_docs ctrlutils ctrlutilseigen)
