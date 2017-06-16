#include "fourier.h"

#include <ctime>
#include <complex>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <vector>

const double EPS = 1e6 + 7;

#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE main_test

#include <boost/test/unit_test.hpp>

fourier::complex_vector
 generate_test(size_t n) {
    fourier::complex_vector vec;
    for (size_t i = 0; i != (1 << n); ++i) {
        vec.push_back(std::complex<double>(static_cast<double> (rand()) / 1000.0,
                                           static_cast<double> (rand()) / 1000.0));
    }
    return vec;
}

fourier::complex_vector
 generate_test(size_t n, double filler) {
    fourier::complex_vector vec;
    for (size_t i = 0; i != (1 << n); ++i) {
        vec.push_back(std::complex<double>(filler, 0));
    }
    return vec;
}

bool is_same(const fourier::complex_vector& v1,
             const fourier::complex_vector& v2) {
    for (size_t i = 0; i != v1.size(); ++i) {
        if (std::abs(v1[i] - v2[i]) > 2 * EPS) {
            return false;
        }
    }
    return true;
}

BOOST_AUTO_TEST_SUITE(TEST_TRANSFORM)

    BOOST_AUTO_TEST_CASE(randomDataTest) {
        std::vector<double> vec = {2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<double> slow_measures;
        std::vector<double> fast_measures;
        std::vector<double> bfast_measures;

        for (auto test_size : vec) {
            fourier::complex_vector test = generate_test(test_size);

            auto slow_res  = fourier::transform              (test);
            auto fast_res  = fourier::fast_transform         (test);
            auto bfast_res = fourier::boosted_fast_transform (test);

            BOOST_CHECK(is_same(slow_res, fast_res));
            BOOST_CHECK(is_same(slow_res, bfast_res));
        }
    }

BOOST_AUTO_TEST_SUITE_END()
