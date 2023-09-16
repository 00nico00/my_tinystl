#include <gtest/gtest.h>

#include "util.hpp"
#include "iterator.hpp"

// TEST(pair_equal_test, make_pair) {
//     auto pair1 = mystl::make_pair(1, 2.0);
//     auto pair2 = mystl::make_pair(1, 2.0);
//     EXPECT_EQ(pair1, pair2);
// }

// TEST(swap_test, swap) {
//     auto a = 5;
//     auto b = 3;
//     mystl::swap(a, b);
//     EXPECT_EQ(a, 3);
//     EXPECT_EQ(b, 5);
// }

// TEST(swap_array_test, swap) {
//     int a[5] = {1, 2, 3, 4, 5};
//     int b[5] = {-1, -2, -3, -4, -5};
//     int b_t[5] = {1, 2, 3, 4, 5};
//     int a_t[5] = {-1, -2, -3, -4, -5};
//     mystl::swap(a, b);
//     for (int i = 0; i < 5; i++) {
//         EXPECT_EQ(a[i], a_t[i]);
//         EXPECT_EQ(b[i], b_t[i]);
//     }
// }

struct X {
    using iterator_category = int;    
};

TEST(iterator_has_iterator_cat, has_iterator_cat) {
    EXPECT_TRUE(mystl::has_iterator_cat<X>::value);
}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}