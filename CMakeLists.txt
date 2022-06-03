cmake_minimum_required(VERSION 3.10)
project (moneybeast-cli VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

include_directories(
    moneybeast-cli PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_executable(
    moneybeast-cli
    "main.cxx"
    "application.cxx"
    "include/main.hxx"
    "include/application.hxx"
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

#!mbsystem


# linking sublibraries with main project
target_link_libraries (
    moneybeast-cli PUBLIC
    mbtypes
    mbsystem
    -lboost_program_options
)
