add_executable(vector_tests vector/tests/unit.cpp)
target_include_directories(vector_tests PRIVATE allocator/src vector/src vector/src/include)

find_package(GTest REQUIRED)
target_link_libraries(vector_tests PRIVATE GTest::GTest GTest::Main)