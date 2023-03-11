#include "lib-integrationtest.hpp"

using namespace IntegrationTestSuite;


TEST(IntegrationTest, Case1){
    RunIntegrationTest(1);
}

TEST(IntegrationTest, Case2){
    RunIntegrationTest(2);
}

TEST(IntegrationTest, Case3){
    RunIntegrationTest(3);
}

TEST(IntegrationTest, Case4){
    RunIntegrationTest(4);
} 


vector<int> main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    
    int result = RUN_ALL_TESTS();

    vector<std::string> new_files = {"../command", "../communication-register", "../log", "../temp",
                                                            "../command0", "../command2", "../command3", "../command4"};

    deleteGarbage(new_files);
    cout << "End of tests" << endl;

    // create an array to store the test results
    std::vector<int> test_results;

    // iterate over the tests and store the results in the array
    const ::testing::UnitTest* const unit_test = ::testing::UnitTest::GetInstance();
    for (int i = 0; i < unit_test->total_test_case_count(); i++) {
        const ::testing::TestCase* const test_case = unit_test->GetTestCase(i);
        for (int j = 0; j < test_case->total_test_count(); j++) {
            const ::testing::TestInfo* const test_info = test_case->GetTestInfo(j);
            if (test_info->result()->Failed()) {
                // test failed, add -1 to the array
                test_results.push_back(-1);
            } else {
                // test passed, add 1 to the array
                test_results.push_back(1);
            }
        }
    }

    // print the test results array
    std::cout << "[";
    for (int i = 0; i < test_results.size(); i++) {
        std::cout << test_results[i];
        if (i < test_results.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "]" << std::endl;
    

    return test_results;
}