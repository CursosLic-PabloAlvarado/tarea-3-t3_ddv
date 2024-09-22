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


filter_client::filter_client() : jack::client() {
    this->b0 = 0;
    this->b1 = 0;
    this->b2 = 0;
    this->a1 = 0;
    this->a2 = 0;
    this->is_biquad_filter_active = false;
    this->biquad_client = new biquad();
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
  if (this->is_biquad_filter_active == true){
    biquad_client->process(nframes, in, out);
  }else{  
    memcpy (out, in, sizeof(sample_t)*nframes);
  }
  return true;
}

void filter_client::set_coeffients(const std::vector<sample_t> coeffients){
    this->b0 = coeffients[0];
    this->b1 = coeffients[1];
    this->b2 = coeffients[2];
    this->a1 = coeffients[4];
    this->a2 = coeffients[5];
    biquad_client->set_coeffients(coeffients);
}

void filter_client::active_biquad_filter(){
  this->is_biquad_filter_active = true;
}
