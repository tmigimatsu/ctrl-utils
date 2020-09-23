############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET Eigen3::Eigen)
    find_package(Eigen3 3.3.7 QUIET)

    if(NOT Eigen3_FOUND)
        init_git_submodule(eigen-git-mirror.git)
        add_subdirectory(eigen-git-mirror.git ${CMAKE_BINARY_DIR}/Eigen3)
        add_library(Eigen3::Eigen INTERFACE IMPORTED GLOBAL)
        target_link_libraries(Eigen3::Eigen INTERFACE eigen)
    else()
        # Set IMPORTED_GLOBAL so parent projects can access Eigen.
        set_target_properties(Eigen3::Eigen PROPERTIES IMPORTED_GLOBAL TRUE)
    endif()
endif()
