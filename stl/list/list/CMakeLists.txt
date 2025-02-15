add_executable(list_tests tests/unit.cpp)

target_link_libraries(list_tests PRIVATE gtest gtest_main)
add_test(NAME list_tests COMMAND list_tests)