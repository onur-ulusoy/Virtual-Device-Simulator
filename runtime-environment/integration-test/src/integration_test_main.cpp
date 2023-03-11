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
    return result;
}