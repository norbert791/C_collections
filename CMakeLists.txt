cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)
project(C_COLLECTIONS
        VERSION 0.5.1
        DESCRIPTION "Library of generic collections"
)

add_subdirectory(hash_table)
add_subdirectory(vector)