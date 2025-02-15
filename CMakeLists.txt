cmake_minimum_required(VERSION 3.31)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

enable_testing()

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

project(freedom)

add_subdirectory(stl)


