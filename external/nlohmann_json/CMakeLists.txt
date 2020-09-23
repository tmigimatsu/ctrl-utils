############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET nlohmann_json::nlohmann_json)
    find_package(nlohmann_json 3.6.1 QUIET)

    if(NOT nlohmann_json_FOUND)
        # Compile without testing.
        set(PREV_BUILD_TESTING ${BUILD_TESTING})
        set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

        init_git_submodule(json.git)
        add_subdirectory(json.git ${CMAKE_BINARY_DIR}/nlohmann_json)

        set(BUILD_TESTING ${PREV_BUILD_TESTING} CACHE BOOL "" FORCE)
    endif()
endif()
