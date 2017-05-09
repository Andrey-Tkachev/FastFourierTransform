#include <vector>
#include <complex>

namespace fourier {
    typedef std::vector<std::complex<double> > complex_vector;
    std::complex<double> calc_base(size_t n);

    void split(const complex_vector& income_vec,
                      complex_vector& first_half,
                      complex_vector& second_half);

    complex_vector
         merge(const complex_vector& first_half,
               const complex_vector& second_half);

    complex_vector
      transform(complex_vector& income_vec);

    complex_vector 
      fast_transform(complex_vector& income_vec);

    complex_vector
      boosted_fast_transform(complex_vector& income_vec);
}