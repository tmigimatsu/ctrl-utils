############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET yaml-cpp::yaml-cpp)
    init_git_submodule(yaml-cpp.git)

    option(YAML_CPP_BUILD_TESTS "" OFF)
    option(YAML_BUILD_SHARED_LIBS "" ON)
    add_subdirectory(yaml-cpp.git)

    add_library(yaml-cpp::yaml-cpp INTERFACE IMPORTED GLOBAL)

    target_link_libraries(yaml-cpp::yaml-cpp INTERFACE
        yaml-cpp)
endif()

function(lib_install_subdirectory_yaml_cpp)
    install(TARGETS yaml-cpp)
endfunction()
