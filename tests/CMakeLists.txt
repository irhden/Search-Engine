cmake_minimum_required(VERSION 3.8)

add_executable(test_SearchEngine test_InvertedIndex.cpp test_SearchServer.cpp)

target_link_libraries(test_SearchEngine PRIVATE search_engine_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(test_SearchEngine)
