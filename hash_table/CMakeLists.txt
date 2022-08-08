cmake_minimum_required(VERSION 3.15)

project("HashTable")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/hash_table.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# without it public headers won't get installed
set(public_headers
    inc/hash_table.h
)

if(CMAKE_COMPILER_IS_GNUC)
    # compile options for this target only
    # treat warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -g)
endif() #TODO: different flags depending on build type

enable_testing()

add_executable(${PROJECT_NAME}_test test/hash_table_unit_tests.c)

target_compile_options(${PROJECT_NAME}_test PRIVATE -O0 -g) #TODO: different flags depending on build type

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_test m)

add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)


# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")

