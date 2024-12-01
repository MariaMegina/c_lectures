#include <gtest/gtest.h>
#include <stdexcept>
#include "exeption.h"

using namespace std;

TEST(TMyExeptionTest, ExeptionA) {
    EXPECT_THROW(
        []() {
            ExeptionA ex;
            ex << "AAAA";
            throw ex;
        }(),
        ExeptionA
    );
}

TEST(TMyExceptionTest, ExeptionB) {
    EXPECT_THROW(
        []() {
            ExeptionB ex;
            ex << "Blya";
            throw ex;
        }(),
        ExeptionB
    );
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

