add_test([=[TestCaseInvertedIndex.TestBasic]=]  D:/_Projects/cpp/search_engine/out/build/x64-debug/tests/test_InvertedIndex.exe [==[--gtest_filter=TestCaseInvertedIndex.TestBasic]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestCaseInvertedIndex.TestBasic]=]  PROPERTIES WORKING_DIRECTORY D:/_Projects/cpp/search_engine/out/build/x64-debug/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[TestCaseInvertedIndex.TestBasic2]=]  D:/_Projects/cpp/search_engine/out/build/x64-debug/tests/test_InvertedIndex.exe [==[--gtest_filter=TestCaseInvertedIndex.TestBasic2]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestCaseInvertedIndex.TestBasic2]=]  PROPERTIES WORKING_DIRECTORY D:/_Projects/cpp/search_engine/out/build/x64-debug/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[TestCaseInvertedIndex.TestInvertedIndexMissingWord]=]  D:/_Projects/cpp/search_engine/out/build/x64-debug/tests/test_InvertedIndex.exe [==[--gtest_filter=TestCaseInvertedIndex.TestInvertedIndexMissingWord]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[TestCaseInvertedIndex.TestInvertedIndexMissingWord]=]  PROPERTIES WORKING_DIRECTORY D:/_Projects/cpp/search_engine/out/build/x64-debug/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  test_InvertedIndex_TESTS TestCaseInvertedIndex.TestBasic TestCaseInvertedIndex.TestBasic2 TestCaseInvertedIndex.TestInvertedIndexMissingWord)
