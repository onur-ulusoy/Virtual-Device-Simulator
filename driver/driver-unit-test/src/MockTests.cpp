#include "lib-unittest.hpp"
using namespace UnitTestSuite;

TEST(MockTest, LibDriverUtilityMockTest){
    LibUtilityMock lu;
    char expectedDevName[] = "test_device";
    EXPECT_CALL(lu, _generateDevName("test_device")).Times(1).WillOnce(Return(expectedDevName));
    EXPECT_EQ(lu.mockGenerateDevName("test_device"), expectedDevName);
}