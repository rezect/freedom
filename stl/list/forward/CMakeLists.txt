add_executable(f_list_tests tests/unit.cpp)

target_link_libraries(f_list_tests PRIVATE gtest gtest_main)
add_test(NAME f_list_tests COMMAND f_list_tests)