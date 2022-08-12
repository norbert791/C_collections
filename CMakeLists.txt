cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(C_COLLECTIONS
        VERSION 0.5.1
        DESCRIPTION "Library of generic collections"
)

OPTION(C_COLLECTIONS_INCLUDE_TEST OFF)

if (C_COLLECTIONS_INCLUDE_TEST)
        enable_testing()
endif()

add_subdirectory(hash_table)
add_subdirectory(vector)

#use -D BUILD_SHARED_LIBS=TRUE for building shared library
#use -D C_COLLECTIONS_INCLUDE_TEST=ON for including library tests