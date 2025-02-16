add_executable(deque tests/unit.cpp)

target_link_libraries(deque PRIVATE gtest gtest_main fmt)
add_test(NAME deque COMMAND deque)