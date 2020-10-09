############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET Eigen3::Eigen)
    find_package(Eigen3 3.3.8 QUIET)

    if(NOT Eigen3_FOUND)
        init_git_submodule(eigen.git)
        add_library(eigen INTERFACE IMPORTED GLOBAL)
        target_include_directories(eigen INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/eigen.git")
        add_library(Eigen3::Eigen ALIAS eigen)
    else()
        # Set IMPORTED_GLOBAL so parent projects can access Eigen.
        set_target_properties(Eigen3::Eigen PROPERTIES IMPORTED_GLOBAL TRUE)
    endif()
endif()
