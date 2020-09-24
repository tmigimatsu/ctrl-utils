############################################################
# CMakeLists for the ctrl-utils library.
#
# Copyright 2020. All Rights Reserved.
#
# Created: September 23, 2020
# Authors: Toki Migimatsu
############################################################

if(NOT TARGET OpenCV::OpenCV)
    find_package(OpenCV REQUIRED)

    add_library(OpenCV::OpenCV INTERFACE IMPORTED GLOBAL)

    target_include_directories(OpenCV::OpenCV INTERFACE
        ${OpenCV_INCLUDE_DIRS})

    target_link_libraries(OpenCV::OpenCV INTERFACE
        ${OpenCV_LIBS})
endif()
