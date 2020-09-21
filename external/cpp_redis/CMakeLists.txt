cmake_minimum_required(VERSION 3.6)

find_package(cpp_redis 4.3.1 QUIET)
if(NOT TARGET cpp_redis::cpp_redis)
    add_library(cpp_redis::cpp_redis INTERFACE IMPORTED GLOBAL)

    find_package(PkgConfig)
    pkg_check_modules(PC_cpp_redis QUIET cpp_redis)
    if(PC_cpp_redis_FOUND)
        target_include_directories(cpp_redis::cpp_redis INTERFACE
            ${PC_cpp_redis_INCLUDE_DIRS})

        find_library(cpp_redis_LIB cpp_redis "${PC_cpp_redis_LIBRARY_DIRS}")
        find_library(tacopie_LIB tacopie "${PC_cpp_redis_LIBRARY_DIRS}")
        target_link_libraries(cpp_redis::cpp_redis INTERFACE
            ${cpp_redis_LIB}
            ${tacopie_LIB}
            pthread)
    else()
        option(BUILD_EXAMPLES "" OFF)
        option(BUILD_TESTS "" OFF)
        set(CMAKE_CXX_FLAGS "-Wno-unused-result")
        add_subdirectory(cpp_redis.git ${CMAKE_BINARY_DIR}/cpp_redis)

        target_link_libraries(cpp_redis::cpp_redis INTERFACE cpp_redis)
        target_include_directories(cpp_redis::cpp_redis INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/cpp_redis.git/includes
            ${CMAKE_CURRENT_SOURCE_DIR}/cpp_redis.git/tacopie/includes)
    endif()
endif()
