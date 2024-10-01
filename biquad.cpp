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
    
    float volume_intensity = this->volume_controller_prt == nullptr ? 1 : this->volume_controller_prt->get_volume_intesity();
    const sample_t *const end_ptr = in + nframes;
    const sample_t *in_ptr = in;

    sample_t *out_ptr = out;
    sample_t y0, y1, y2, y3, y4, y5, y6, y7 = 0; 

    while (in_ptr != end_ptr) {
        y0 =  (this->b0 * *(in_ptr)  +  this->b1 * x_past_1 + this->b2 * x_past_2 - this->a1 * y_past_1 - this->a2 * y_past_2);
        y1 =  (this->b0 * *(in_ptr + 1)  +  this->b1 * *(in_ptr) + this->b2 * x_past_1 - this->a1 * y0 - this->a2 * y_past_1);
        y2 =  (this->b0 * *(in_ptr + 2)  +  this->b1 * *(in_ptr + 1) + this->b2 * *(in_ptr) - this->a1 * y1 - this->a2 * y0);
        y3 =  (this->b0 * *(in_ptr + 3)  +  this->b1 * *(in_ptr + 2) + this->b2 * *(in_ptr + 1) - this->a1 * y2 - this->a2 * y1);
        y4 =  (this->b0 * *(in_ptr + 4)  +  this->b1 * *(in_ptr + 3) + this->b2 * *(in_ptr + 2) - this->a1 * y3 - this->a2 * y2);
        y5 =  (this->b0 * *(in_ptr + 5)  +  this->b1 * *(in_ptr + 4) + this->b2 * *(in_ptr + 3) - this->a1 * y4 - this->a2 * y3);
        y6 =  (this->b0 * *(in_ptr + 6)  +  this->b1 * *(in_ptr + 5) + this->b2 * *(in_ptr + 4) - this->a1 * y5 - this->a2 * y4);
        y7 =  (this->b0 * *(in_ptr + 7)  +  this->b1 * *(in_ptr + 6) + this->b2 * *(in_ptr + 5) - this->a1 * y6 - this->a2 * y5);

        // update states
        this->x_past_2 = *(in_ptr + 6);
        this->x_past_1 = *(in_ptr + 7);
        this->y_past_2 = y6;
        this->y_past_1 = y7;

        *(out_ptr++) = volume_intensity * y0;
        *(out_ptr++) = volume_intensity * y1;
        *(out_ptr++) = volume_intensity * y2;
        *(out_ptr++) = volume_intensity * y3;
        *(out_ptr++) = volume_intensity * y4;
        *(out_ptr++) = volume_intensity * y5;
        *(out_ptr++) = volume_intensity * y6;
        *(out_ptr++) = volume_intensity * y7;

        // update pointer
        in_ptr += 8;
    }
    
  return true;
}

void biquad::set_coefficients(const std::vector<sample_t> coeffients){
    this->b0 = coeffients[0];
    this->b1 = coeffients[1];
    this->b2 = coeffients[2];
    this->a1 = coeffients[4];
    this->a2 = coeffients[5];
}

void biquad::set_volume_controller(volume_controller* volume_controller){
    this->volume_controller_prt = volume_controller;
}