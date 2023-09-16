#include <gtest/gtest.h>

#include "util.hpp"

TEST(pair_equal_test, make_pair) {
    auto pair1 = mystl::make_pair(1, 2.0);
    auto pair2 = mystl::make_pair(1, 2.0);
    EXPECT_EQ(pair1, pair2);
}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}