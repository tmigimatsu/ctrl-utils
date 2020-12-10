############################################################
# CMakeLists for the ctrl_utils library.
#
# Copyright 2018. All Rights Reserved.
#
# Created: May 7, 2018
# Authors: Toki Migimatsu
############################################################

set(LIB_NAME ${PROJECT_NAME})

# Create the library.
add_library(${LIB_NAME} INTERFACE)
add_library(${LIB_NAME}::${LIB_NAME} ALIAS ${LIB_NAME})

# Enable clang tidy checks.
if(${LIB_CMAKE_NAME}_CLANG_TIDY)
    target_enable_clang_tidy(${LIB_NAME})
endif()

# Set include directories.
set(LIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${LIB_NAME}
  INTERFACE
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "$<BUILD_INTERFACE:${LIB_INCLUDE_DIR}>"
)

# Install the library.
install(TARGETS ${LIB_NAME}
    EXPORT ${LIB_NAME}Targets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# Install the headers.
install(DIRECTORY "${LIB_INCLUDE_DIR}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Create LibConfig.cmake
set(LIB_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}")
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${LIB_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    INSTALL_DESTINATION "${LIB_CONFIG_INSTALL_DIR}"
)

# Create LibConfigVersion.cmake.
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Install LibConfig.cmake and LibConfigVersion.cmake.
install(FILES
    "${PROJECT_BINARY_DIR}/${LIB_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake"
    DESTINATION ${LIB_CONFIG_INSTALL_DIR}
)

# Export targets to LibTargets.cmake.
install(EXPORT ${LIB_NAME}Targets
    DESTINATION ${LIB_CONFIG_INSTALL_DIR}
    NAMESPACE ${LIB_NAME}::
)

# Export targets to LibTargets.cmake for the build tree.
export(TARGETS ${LIB_NAME}
    NAMESPACE ${LIB_NAME}::
    FILE ${LIB_NAME}Targets.cmake
)
