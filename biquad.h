/**
 * biquad.h
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

#ifndef _BIQUAD_H
#define _BIQUAD_H

#include <iostream>
#include <cstring>
#include <vector>
#include "jack_client.h"
#include "volume_controller.h"


/**
 * Jack client class
 *
 * This class wraps some basic jack functionality.
 */
class biquad : public jack::client {

private:
    double a1,a2,b0,b1,b2;
    double x1,x2,y1,y2;

  /**
   * Pointer to the volume controller.
   * This member points to an instance of the volume_controller class that 
   * manages volume operations.
  */
    volume_controller* volume_controller_prt;
  
    
public:
    // typedef jack::client::sample_t sample_t;

    /**
     * The default constructor performs some basic connections.
     */
    biquad(volume_controller* volume_ptr);
    ~biquad();

    /**
     * Passthrough functionality
     */
    virtual bool process(jack_nframes_t nframes,
                        const sample_t *const in,
                        sample_t *const out) override;

    void set_coeffients(const std::vector<sample_t> coeffients);
    void set_volume_controller(volume_controller* volume_controller);
};


#endif
