#pragma once

#include <iostream>

#include "util.hpp"

namespace mystl {

void test_pair() {
    auto pair1 = mystl::make_pair(1, 5.2);
    auto pair2 = mystl::make_pair(1, 4.0);

    std::cout << std::boolalpha << (pair1 > pair2) << '\n';
}

}  // namespace mystl