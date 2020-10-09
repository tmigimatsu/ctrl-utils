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

        # Disable Eigen tests.
        set(TESTING_FILES "eigen.git/CMakeLists.txt eigen.git/unsupported/CMakeLists.txt eigen.git/blas/CMakeLists.txt")
        execute_process(
            COMMAND sed -i "s/(BUILD_TESTING/(EIGEN_BUILD_TESTING/g" "${TESTING_FILES}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
        option(EIGEN_BUILD_TESTING "" OFF)

        add_subdirectory(eigen.git)
        add_library(Eigen3::Eigen INTERFACE IMPORTED GLOBAL)
        target_link_libraries(Eigen3::Eigen INTERFACE eigen)
    else()
        # Set IMPORTED_GLOBAL so parent projects can access Eigen.
        set_target_properties(Eigen3::Eigen PROPERTIES IMPORTED_GLOBAL TRUE)
    endif()
endif()
