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


int main(int argc, char **argv) {
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
    cout << "[ ";
    for (int i = 0; i < test_results.size(); i++) {
        if (test_results[i] == 1) {
            cout << "\033[32m";
            cout << "✓ ";
        } else {
            cout << "\033[31m";
            cout << "✗ ";
        }
        if (i < test_results.size() - 1) {
            std::cout << " ";
        }
    }
    cout << "\033[0m";
    cout << "]" << std::endl;

    ofstream file("test_results.txt");
    for (int i = 0; i < test_results.size(); i++) {
        if (test_results[i] == 1) {
            file << "✓\n";
        } else {
            file << "✗\n";
        }
    }
    file.close();
    

    return result;
}