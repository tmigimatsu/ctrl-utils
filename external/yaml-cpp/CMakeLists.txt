############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET yaml-cpp::yaml-cpp)
    find_package(yaml-cpp 0.6.3 QUIET)
    if(NOT yaml-cpp_FOUND)
        option(YAML_CPP_BUILD_TESTS "" OFF)
        option(YAML_BUILD_SHARED_LIBS "" ON)

        init_git_submodule(yaml-cpp.git)
        add_subdirectory(yaml-cpp.git ${CMAKE_BINARY_DIR}/yaml-cpp)
    endif()

    add_library(yaml-cpp::yaml-cpp INTERFACE IMPORTED GLOBAL)

    target_link_libraries(yaml-cpp::yaml-cpp INTERFACE
        yaml-cpp)
endif()
