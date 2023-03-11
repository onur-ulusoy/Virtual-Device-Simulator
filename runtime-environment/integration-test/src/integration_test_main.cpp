#include "lib-integrationtest.hpp"

using namespace IntegrationTestSuite;


TEST(IntegrationTest, Case2){
    RunIntegrationTest(1);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();

}