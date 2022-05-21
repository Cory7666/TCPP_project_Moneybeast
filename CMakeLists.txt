cmake_minimum_required(VERSION 3.10)
project (moneybeast-cli VERSION 0.1.0)

include_directories(
    moneybeast-cli PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/"
)

add_executable(
    moneybeast-cli
    main.cxx
    mbtypes.cxx
    include/main.hxx
    include/mbtypes.hxx
)
