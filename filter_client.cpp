/**
 * filter_client.cpp
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

#include "filter_client.h"


filter_client::filter_client(volume_controller* volume) : jack::client() {
    this->is_biquad_filter_active = false;
    this->is_passall_filter_active = true;
    this->biquad_client = new biquad(volume);
    this->cascade_2.set_volume(volume);
    this->cascade_3.set_volume(volume);
    this->volume_controller_prt = volume;
}

filter_client::~filter_client() {
}
  
/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
   * the user (e.g. using Ctrl-C on a unix-ish operating system)
   */
bool filter_client::process(jack_nframes_t nframes,
                                 const sample_t *const in,
                                 sample_t *const out) {
  if (this->is_cascade_filter_active){
    if (is_cascade_2_active) {
            cascade_2.process(nframes, in, out);
        } else {
            cascade_3.process(nframes, in, out);
        }
  } else if (this->is_biquad_filter_active){
    biquad_client->process(nframes, in, out);

  }else if (this->is_passall_filter_active){  
    float volume_intensity = this->volume_controller_prt->get_volume_intesity();
    const sample_t *const end_ptr = in + nframes;
    const sample_t *in_ptr = in;
    sample_t *out_ptr = out;

    for (;in_ptr != end_ptr;){
        *out_ptr = volume_intensity * (*in_ptr);
        in_ptr++;
        out_ptr++;
    }
  }
  return true;
}

void filter_client::set_coeffients(const std::vector<std::vector<sample_t>> coeffients) {
    biquad_client->set_coefficients(coeffients[0]);
    if (coeffients.size() == 2) {
        cascade_2.set_coefficients(coeffients);
        this->is_cascade_2_active = true; 
    } else {
        cascade_3.set_coefficients(coeffients);
        this->is_cascade_2_active = false;
    }
}

void filter_client::active_biquad_filter() {
    std::cout << "Active biquad" << std::endl;
    this->is_biquad_filter_active = true;
    this->is_cascade_filter_active = false;
    this->is_passall_filter_active = false;
}
void filter_client::active_cascade_filter() {
    std::cout << "Active cascade" << std::endl;
    this->is_cascade_filter_active = true;
    this->is_biquad_filter_active = false;
    this->is_passall_filter_active = false;
}
void filter_client::active_passall_filter() {
    std::cout << "Active pass all" << std::endl;
    this->is_passall_filter_active = true;
    this->is_biquad_filter_active = false;
    this->is_cascade_filter_active = false;
}
