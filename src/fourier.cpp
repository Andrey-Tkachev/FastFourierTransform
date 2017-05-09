#include "fourier.h"

namespace fourier {
    std::complex<double> calc_base(size_t n) {
        double phi = -6.28f / static_cast<double> (n);
        double re = std::cos(phi); 
        double im = -std::sin(phi);
        return std::complex<double> (re, im);
    }

    void split(const complex_vector& income_vec,
                      complex_vector& first_half,
                      complex_vector& second_half) {
        size_t input_size = income_vec.size();
        first_half.reserve(input_size / 2);
        second_half.reserve(input_size - input_size / 2);
        for (size_t i = 0; i != input_size; ++i) {
            if (i % 2 == 0) {
                first_half.push_back(income_vec[i]);
            } else {
                second_half.push_back(income_vec[i]);
            }
        }
    }

    complex_vector
       merge(const complex_vector& first_half,
               const complex_vector& second_half) {
        size_t f_size = first_half.size();
        size_t s_size = second_half.size();
        size_t input_size = f_size + s_size;

        std::complex<double> base(calc_base(input_size));
        complex_vector result(input_size);

        std::complex<double> power_of_base(1.0, 0.0);
        for (size_t i = 0; i != input_size / 2; ++i) {
            std::complex<double> a = first_half[i % f_size];
            std::complex<double> b = second_half[i % f_size] * power_of_base;
            result[i] = a + b;
            result[i + input_size / 2] = a - b;
            power_of_base *= base;
        }

        return result;
    }

    complex_vector
      transform(complex_vector& income_vec) {
        size_t input_size = income_vec.size();
        std::complex<double> base(calc_base(input_size));
        complex_vector base_pows(input_size);
        base_pows[0] = 1.0;
        for (size_t i = 1; i != input_size; ++i) {
            base_pows[i] = base_pows[i - 1] * base;
        }
        complex_vector result(input_size);
        std::complex<double> base_pow(1);
        for (size_t i = 0; i != input_size; ++i) {
            for (size_t j = 0; j != input_size; ++j) {
                result[i] += income_vec[j] * base_pows[(i * j) % input_size];
            }
        }
        return result;
    }

    complex_vector 
      fast_transform(complex_vector& income_vec) {
        if (income_vec.size() == 1) {
            return income_vec;
        }

        complex_vector first_half;
        complex_vector second_half;

        split(income_vec, first_half, second_half);
        first_half = fast_transform(first_half);
        second_half = fast_transform(second_half);

        return merge(first_half, second_half);
    }

    complex_vector
      boosted_fast_transform(complex_vector& income_vec) {
        if (income_vec.size() < 32) {
            return transform(income_vec);
        }

        complex_vector first_half;
        complex_vector second_half;

        split(income_vec, first_half, second_half);
        first_half = boosted_fast_transform(first_half);
        second_half = boosted_fast_transform(second_half);

        return merge(first_half, second_half);
    }
}
