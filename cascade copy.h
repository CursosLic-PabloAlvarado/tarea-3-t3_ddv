/**
 * cascade copy.h
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

#ifndef _CASCADE_COPY_H
#define _CASCADE_COPY_H

#include <iostream>
#include <cstring>
#include <vector>
#include "jack_client.h"
#include "volume_controller.h"
#include "biquad.h"


/**
 * Jack client class
 *
 * This class wraps some basic jack functionality.
 */
class cascade : public jack::client {

private:
  /**
   * Pointer to the volume controller.
   * This member points to an instance of the volume_controller class that 
   * manages volume operations.
  */
    volume_controller* volume_controller_prt;

    biquad *biquad_client1;
    biquad *biquad_client2;
    biquad *biquad_client3;

    int num_biquads;
    
public:

    /**
     * The default constructor performs some basic connections.
     */
    cascade();
    cascade(volume_controller* volume_ptr);
    ~cascade();

    /**
     * Cascadde functionality
     */
    virtual bool process(jack_nframes_t nframes,
                        const sample_t *const in,
                        sample_t *const out) override;

    void set_coefficients(const std::vector<std::vector<sample_t>> coefficients);
};

#endif
