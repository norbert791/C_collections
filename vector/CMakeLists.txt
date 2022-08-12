cmake_minimum_required(VERSION 3.15)

project("Vector")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME})

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    SET(${PROJECT_NAME}_FLAGS -Weverything -pedantic)
    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        SET(${PROJECT_NAME}_FLAGS ${${PROJECT_NAME}_FLAGS} -O0 -g)
    else()
        SET(${PROJECT_NAME}_FLAGS ${${PROJECT_NAME}_FLAGS} -O3)
    endif()
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    SET(${PROJECT_NAME}_FLAGS -Wall -Wextra -ansi -pedantic)
    if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
        SET(${PROJECT_NAME}_FLAGS ${${PROJECT_NAME}_FLAGS} -O0 -g)
    else()
        SET(${PROJECT_NAME}_FLAGS ${${PROJECT_NAME}_FLAGS} -O3)
    endif()
endif()

target_sources(${PROJECT_NAME}
    PRIVATE
        src/vector.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set(public_headers
    inc/vector.h
)

enable_testing()

if (C_COLLECTIONS_INCLUDE_TEST)
    add_executable(${PROJECT_NAME}_test test/test_vector.c test/test.c)

    target_compile_options(${PROJECT_NAME}_test PRIVATE ${${PROJECT_NAME}_FLAGS})

    target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME}_test m)

    add_test(${PROJECT_NAME}_test ${PROJECT_NAME}_test)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
