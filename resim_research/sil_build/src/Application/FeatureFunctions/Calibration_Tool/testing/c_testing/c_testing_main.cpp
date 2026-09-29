#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

int main(int argc, char* argv[])
{   
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}