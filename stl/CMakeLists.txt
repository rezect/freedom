# Vector
add_executable(vector_tests vector/tests/unit.cpp)

target_link_libraries(vector_tests PRIVATE gtest gtest_main)
target_include_directories(vector_tests PRIVATE allocator/src vector/src vector/src/include)

add_test(NAME vector_tests COMMAND vector_tests)

# List
add_subdirectory(list)

# Tree
add_subdirectory(tree)

# Abstract
add_subdirectory(abstract)
