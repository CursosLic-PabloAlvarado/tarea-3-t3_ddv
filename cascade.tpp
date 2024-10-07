#include "cascade.h"

#include <cstring>

template <unsigned int N>
cascade<N>::cascade():
    biquad_filters(N) {  // Initialize vector with N biquad filters
}

template <unsigned int N>
bool cascade<N>::process(jack_nframes_t nframes,
                         const sample_t *const in,
                         sample_t *const out) {

    sample_t* temp_in = const_cast<sample_t*>(in);  // Input buffer, potentially modified by filters
    sample_t* temp_out = out;                       // Output buffer

    // Process the first biquad filter
    this->biquad_filters[0].process(nframes, temp_in, temp_in);

    // Process remaining biquads
    if constexpr (N == 2) {
        // Unrolled for 2 filters
        this->biquad_filters[1].process(nframes, temp_in, temp_out);
    } else if constexpr(N == 3) {
        // Unrolled for 3 filters
        this->biquad_filters[1].process(nframes, temp_in, temp_in); // Process first in-place
        this->biquad_filters[2].process(nframes, temp_in, temp_out); // Process last to output
    } 

    return true;
}

/**
 * Set the coefficients for all biquad filters in the cascade.
 */
template <unsigned int N>
void cascade<N>::set_coefficients(const std::vector<std::vector<sample_t>>& coefficients) {
    if (coefficients.size() != N) {
        std::cerr << "Error: Number of coefficient sets must match the number of biquads." << std::endl;
    }
    if constexpr (N == 2) {
        this->biquad_filters[0].set_coefficients(coefficients[0]);
        this->biquad_filters[0].set_volume_controller(this->volume_controller_prt);
        this->biquad_filters[1].set_coefficients(coefficients[1]);
        this->biquad_filters[1].set_volume_controller(this->volume_controller_prt);
    }
    else if constexpr (N == 3) {
        // std::cout << "set coeffs for 3rd order" << std::endl;
        this->biquad_filters[0].set_coefficients(coefficients[0]);
        this->biquad_filters[0].set_volume_controller(this->volume_controller_prt);
        this->biquad_filters[1].set_coefficients(coefficients[1]);
        this->biquad_filters[1].set_volume_controller(this->volume_controller_prt);
        this->biquad_filters[2].set_coefficients(coefficients[2]);
        this->biquad_filters[2].set_volume_controller(this->volume_controller_prt);
    }
}

/**
 * Set the volumen conttroller
 */
template <unsigned int N>
void cascade<N>::set_volume(volume_controller* volume_ptr) {
        this->volume_controller_prt = volume_ptr;
    
}