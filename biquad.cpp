/**
 * biquad.cpp
 *
 * Copyright (C) 2023-2024  Pablo Alvarado
 * EL5805 Procesamiento Digital de Señales
 * Escuela de Ingeniería Electrónica
 * Tecnológico de Costa Rica
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "biquad.h"

#include <cstring>
#include <immintrin.h> 

biquad::biquad() : jack::client() {
    this->b0 = 0;
    this->b1 = 0;
    this->b2 = 0;
    this->a1 = 0;
    this->a2 = 0;
    this->x_past_1 = 0;
    this->x_past_2 = 0;
    this->y_past_1 = 0;
    this->y_past_2 = 0;
    this->volume_controller_prt = nullptr;
}

biquad::biquad(volume_controller* volume_controller) : jack::client() {
    this->b0 = 0;
    this->b1 = 0;
    this->b2 = 0;
    this->a1 = 0;
    this->a2 = 0;
    this->x_past_1 = 0;
    this->x_past_2 = 0;
    this->y_past_1 = 0;
    this->y_past_2 = 0;
    this->volume_controller_prt = volume_controller;
}


biquad::~biquad() {
}
  
/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
   * the user (e.g. using Ctrl-C on a unix-ish operating system)
   */
bool biquad::process(jack_nframes_t nframes,
                     const sample_t *const in,
                     sample_t *const out) {
    
    // Setting pointers for sample managment
    float volume_intensity = this->volume_controller_prt == nullptr ? 1 : this->volume_controller_prt->get_volume_intesity();
    const sample_t *const end_ptr = in + nframes;
    const sample_t *in_ptr = in;
    sample_t *out_ptr = out;

    // Declaring variables for the direct form II coefficients
    __m256 a1_vec = _mm256_set1_ps(this->a1);
    __m256 a2_vec = _mm256_set1_ps(this->a2);
    __m256 b0_vec = _mm256_set1_ps(this->b0);
    __m256 b1_vec = _mm256_set1_ps(this->b1);
    __m256 b2_vec = _mm256_set1_ps(this->b2);
    __m256 volume_vec = _mm256_set1_ps(volume_intensity);
    
    // Declaring variables for the direct form II necessary constants
    __m256 x_past_1_vec = _mm256_set1_ps(this->x_past_1);
    __m256 x_past_2_vec = _mm256_set1_ps(this->x_past_2);

    // Performing the processing through loop unrolling
    while (in_ptr != end_ptr) {
        __m256 in_vec = _mm256_loadu_ps(in_ptr);

        // Initialize state variables
        __m256 w0_vec = _mm256_sub_ps(in_vec, _mm256_add_ps(_mm256_mul_ps(a1_vec, x_past_1_vec), _mm256_mul_ps(a2_vec, x_past_2_vec)));
        __m256 w1_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 1), _mm256_add_ps(_mm256_mul_ps(a1_vec, w0_vec), _mm256_mul_ps(a2_vec, x_past_1_vec)));
        __m256 w2_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 2), _mm256_add_ps(_mm256_mul_ps(a1_vec, w1_vec), _mm256_mul_ps(a2_vec, w0_vec)));
        __m256 w3_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 3), _mm256_add_ps(_mm256_mul_ps(a1_vec, w2_vec), _mm256_mul_ps(a2_vec, w1_vec)));
        __m256 w4_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 4), _mm256_add_ps(_mm256_mul_ps(a1_vec, w3_vec), _mm256_mul_ps(a2_vec, w2_vec)));
        __m256 w5_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 5), _mm256_add_ps(_mm256_mul_ps(a1_vec, w4_vec), _mm256_mul_ps(a2_vec, w3_vec)));
        __m256 w6_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 6), _mm256_add_ps(_mm256_mul_ps(a1_vec, w5_vec), _mm256_mul_ps(a2_vec, w4_vec)));
        __m256 w7_vec = _mm256_sub_ps(_mm256_loadu_ps(in_ptr + 7), _mm256_add_ps(_mm256_mul_ps(a1_vec, w6_vec), _mm256_mul_ps(a2_vec, w5_vec)));

        // Output samples
        __m256 y0_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w0_vec), _mm256_mul_ps(b1_vec, x_past_1_vec)), _mm256_mul_ps(b2_vec, x_past_2_vec));
        __m256 y1_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w1_vec), _mm256_mul_ps(b1_vec, w0_vec)), _mm256_mul_ps(b2_vec, x_past_1_vec));
        __m256 y2_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w2_vec), _mm256_mul_ps(b1_vec, w1_vec)), _mm256_mul_ps(b2_vec, w0_vec));
        __m256 y3_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w3_vec), _mm256_mul_ps(b1_vec, w2_vec)), _mm256_mul_ps(b2_vec, w1_vec));
        __m256 y4_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w4_vec), _mm256_mul_ps(b1_vec, w3_vec)), _mm256_mul_ps(b2_vec, w2_vec));
        __m256 y5_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w5_vec), _mm256_mul_ps(b1_vec, w4_vec)), _mm256_mul_ps(b2_vec, w3_vec));
        __m256 y6_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w6_vec), _mm256_mul_ps(b1_vec, w5_vec)), _mm256_mul_ps(b2_vec, w4_vec));
        __m256 y7_vec = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(b0_vec, w7_vec), _mm256_mul_ps(b1_vec, w6_vec)), _mm256_mul_ps(b2_vec, w5_vec));

        // Update states
        x_past_2_vec = w6_vec;
        x_past_1_vec = w7_vec;

        // Output samples
        _mm256_storeu_ps(out_ptr, _mm256_mul_ps(y0_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 1, _mm256_mul_ps(y1_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 2, _mm256_mul_ps(y2_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 3, _mm256_mul_ps(y3_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 4, _mm256_mul_ps(y4_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 5, _mm256_mul_ps(y5_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 6, _mm256_mul_ps(y6_vec, volume_vec));
        _mm256_storeu_ps(out_ptr + 7, _mm256_mul_ps(y7_vec, volume_vec));

        // Update pointers
        in_ptr += 8;
        out_ptr += 8;
    }
    
    return true;
}

/**
* Method used to set coefficients from the obtained filter matrix
*/
void biquad::set_coefficients(const std::vector<sample_t> coeffients){
    this->b0 = coeffients[0];
    this->b1 = coeffients[1];
    this->b2 = coeffients[2];
    this->a1 = coeffients[4];
    this->a2 = coeffients[5];
}

/**
* Method used to set the value for the desired volume on the output 
*/
void biquad::set_volume_controller(volume_controller* volume_controller){
    this->volume_controller_prt = volume_controller;
}