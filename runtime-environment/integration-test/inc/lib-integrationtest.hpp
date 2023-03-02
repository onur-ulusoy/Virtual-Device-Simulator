#ifndef INTEGRATIONTEST_LIB_INTEGRATION_HPP
#define INTEGRATIONTEST_LIB_INTEGRATION_HPP

#include <fstream>
#include <gtest/gtest.h>
#include "libdriver.hpp"
#include "libtesterutility.hpp"

using namespace DeviceSim;
//namespace Tests-Vds{
namespace IntegrationTestSuite{

    enum ComparisonMode {
            LOG,
            REGISTER_LOG,
            DEFAULT_MODE
    };

    bool areFilesEqual(const string& filePath1, const string& filePath2, ComparisonMode mode);
    void deleteGarbage(vector<string> filenames);
    void RunIntegrationTest(int CaseNum);  
}

#endif