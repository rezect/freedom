cmake_minimum_required(VERSION 3.31)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

project(freedom)

add_subdirectory(stl)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

