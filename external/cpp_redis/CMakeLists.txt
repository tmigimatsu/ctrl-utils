############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET cpp_redis::cpp_redis)
    add_library(cpp_redis::cpp_redis INTERFACE IMPORTED GLOBAL)
    execute_process(
        COMMAND sed -i "s/^#include <cpp_redis\\/misc\\/macro.hpp>/#include <cpp_redis\\/misc\\/macro.hpp>\\n#include <thread>/" cpp_redis.git/sources/core/client.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    # Try to find cpp_redis via PkgConfig.
    find_package(PkgConfig)
    pkg_check_modules(PC_cpp_redis QUIET cpp_redis)
    if(PC_cpp_redis_FOUND)
        # Extract cpp_redis from PkgConfig.
        target_include_directories(cpp_redis::cpp_redis INTERFACE
            ${PC_cpp_redis_INCLUDE_DIRS})

        find_library(cpp_redis_LIB cpp_redis "${PC_cpp_redis_LIBRARY_DIRS}")
        find_library(tacopie_LIB tacopie "${PC_cpp_redis_LIBRARY_DIRS}")
        target_link_libraries(cpp_redis::cpp_redis INTERFACE
            ${cpp_redis_LIB}
            ${tacopie_LIB}
            pthread)
    else()
        # Compile cpp_redis.
        set(BUILD_EXAMPLES false)
        set(BUILD_TESTS false)
        set(CMAKE_CXX_FLAGS "-Wno-unused-result")

        init_git_submodule(cpp_redis.git RECURSIVE)
        add_subdirectory(cpp_redis.git)
        set_target_properties(cpp_redis PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )
        set_target_properties(tacopie PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )

        target_link_libraries(cpp_redis::cpp_redis INTERFACE cpp_redis)

        target_include_directories(cpp_redis::cpp_redis INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/cpp_redis.git/includes
            ${CMAKE_CURRENT_SOURCE_DIR}/cpp_redis.git/tacopie/includes)
    endif()
endif()
