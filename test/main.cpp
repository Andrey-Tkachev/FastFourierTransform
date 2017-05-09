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
const bool LOG_SCALE = false;

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

template <typename TMethod>
std::pair<double, fourier::complex_vector>
 time_measure(fourier::complex_vector& test_data,
              TMethod func) {
    std::clock_t timer = std::clock();
    fourier::complex_vector result = func(test_data);
    double time = 1e3 * static_cast<double>(std::clock() - timer) / CLOCKS_PER_SEC;
    if (LOG_SCALE)
        time = std::log2(time);
    return make_pair(time, result); 
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

void log_measures(std::vector<double>& measures,
                  std::vector<double>& dataset_sizes,
                  std::string name) {
    std::ofstream fout(name + ".csv");
    for (size_t i = 0; i < measures.size(); ++i) {
        fout << "2^" << dataset_sizes[i] << "; " << measures[i] << "\n";
    }
    fout.close();
}

int main() {
    std::ios::sync_with_stdio(false);
    std::vector<double> vec = {2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> slow_measures;
    std::vector<double> fast_measures;
    std::vector<double> bfast_measures;

    for (auto test_size : vec) {
        std::cout << "Test size: "<< (1 << static_cast<int>(test_size)) << " elements" << std::endl;
        fourier::complex_vector test = generate_test(test_size);

        auto slow_res = time_measure(test, fourier::fast_transform);
        std::cout << "Simple transform done. " << slow_res.first << "ms\n";

        auto fast_res = time_measure(test, fourier::fast_transform);
        std::cout << "Fast   transform done. " << fast_res.first << "ms\n";

        auto bfast_res = time_measure(test, fourier::boosted_fast_transform);
        std::cout << "BFast  transform done. " << bfast_res.first << "ms\n";

        if (is_same(slow_res.second, fast_res.second) &&
            is_same(slow_res.second, bfast_res.second)) {
            slow_measures.push_back(slow_res.first);
            fast_measures.push_back(fast_res.first);
            bfast_measures.push_back(bfast_res.first);
        } else {
            throw std::runtime_error("Error on dataset " +  std::to_string(test_size));
        }
        std::cout << "\n";
    }

    log_measures(slow_measures, vec, "simple");
    log_measures(fast_measures, vec, "fast");
    log_measures(bfast_measures, vec, "boosted_fast");
}
