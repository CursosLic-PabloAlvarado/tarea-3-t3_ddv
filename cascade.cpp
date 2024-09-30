/**
 * cascade.cpp
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

#include "cascade.h"

#include <cstring>

cascade::cascade() : jack::client() {
    this->volume_controller_prt = nullptr;
    this->biquad_client1 = new biquad(nullptr);
    this->biquad_client2 = new biquad(nullptr);
    this->biquad_client3 = new biquad(nullptr);
}

cascade::cascade(volume_controller* volume_controller) : jack::client() {
    this->volume_controller_prt = volume_controller;
    this->biquad_client1 = new biquad(nullptr);
    this->biquad_client2 = new biquad(nullptr);
    this->biquad_client3 = new biquad(nullptr);
}


cascade::~cascade() {
}
  
/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client does nothing more than copy data from its input
 * port to its output port. It will exit when stopped by 
   * the user (e.g. using Ctrl-C on a unix-ish operating system)
   */
bool cascade::process(jack_nframes_t nframes,
                                 const sample_t *const in,
                                 sample_t *const out) {
    
    // Llamar a biquad clients
    // memcpy (out, in, sizeof(sample_t)*nframes);
    sample_t *const out1 = new sample_t[nframes];
    sample_t *const out2 = new sample_t[nframes];   
    switch (this->num_biquads)
    {
    case 1:
        biquad_client1->process(nframes, in, out);
        break;
    case 2:
        biquad_client1->process(nframes, in, out1);
        biquad_client2->process(nframes, out1, out);
        break;
    case 3:
        biquad_client1->process(nframes, in, out1);
        biquad_client2->process(nframes, out1, out2);
        biquad_client3->process(nframes, out2, out);
        break;
    default:
        break;
    }

    return true;
}

void cascade::set_coefficients(const std::vector<std::vector<sample_t>> coeffients){
    // set coeffients for biquad clients
    //example:
    //biquad_client->set_coeffients(coeffients[0]);
    this->num_biquads = coeffients.size();

    switch (this->num_biquads)
    {
    case 1:
        biquad_client1->set_coefficients(coeffients[0]);
        biquad_client1->set_volume_controller(this->volume_controller_prt);
        break;
    case 2:
        biquad_client1->set_coefficients(coeffients[0]);
        biquad_client2->set_coefficients(coeffients[1]);
        biquad_client2->set_volume_controller(this->volume_controller_prt);
        break;
    case 3:
        biquad_client1->set_coefficients(coeffients[0]);
        biquad_client2->set_coefficients(coeffients[1]);
        biquad_client3->set_coefficients(coeffients[2]);
        biquad_client3->set_volume_controller(this->volume_controller_prt);
        break;
    default:
        break;
    }

}