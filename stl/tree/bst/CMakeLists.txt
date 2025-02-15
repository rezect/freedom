add_executable(tree_bst_tests tests/unit.cpp)

target_link_libraries(tree_bst_tests PRIVATE gtest gtest_main fmt)
add_test(NAME tree_bst_tests COMMAND tree_bst_tests)