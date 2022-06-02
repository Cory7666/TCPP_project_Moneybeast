cmake_minimum_required(VERSION 3.10)
project (moneybeast-cli VERSION 0.1.0)

include_directories(
    moneybeast-cli PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_executable(
    moneybeast-cli
    "main.cxx"
    "include/main.hxx"
)



############################
### Sublibrary with types ##
############################

include_directories (
    mbtypes PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_library (
    mbtypes
    "include/mbtypes.hxx"
    "mbtypes.cxx"
)

#!mbtypes

###################################
### Sublibrary with system logic ##
###################################

include_directories (
    mbsystem PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_library (
    mbsystem
    "include/mbsystem.hxx"
    "mbsystem.cxx"
)

#!mbtypes


# linking sublibraries with main project
target_link_libraries (
    moneybeast-cli PUBLIC
    mbtypes
    mbsystem
)
