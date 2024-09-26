/**
 * volume_controller.h
 *
 * Copyright (C) 2023-2024  Pablo Alvarado, Dario Gutierrez, Valeskka Blanco, Diana Mejias
 * EL5805 Digital Signal Processing
 * School of Electronic Engineering
 * Costa Rica Institute of Technology
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
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

#ifndef _VOLUME_CONTROLLER_H
#define _VOLUME_CONTROLLER_H

#include "jack_client.h"

/**
 * This class manages the volume of audio signals. It provides methods for 
 * activating and deactivating volume control, increasing or decreasing 
 * the volume in decibels (dB), and converting the volume from dB to intensity.
 */
class volume_controller {

private:
    
    //This represents the intensity of the signal
    float volume_intesity = 1;

    //Represents the current volume level in dB.
    int volume_db = 1;

     // Represents if volume controller is active (true)
    bool is_active;

    
    // Converts the volume from decibels (dB) to intensity.
    void db_conversion();

public:
    /**
     * Constructor for the volume_controller class.
     * Initializes the volume controller with default settings.
     */
    volume_controller();

    /**
     * Destructor for the volume_controller class.
     * Cleans up any resources used by the volume controller. Since no dynamic
     * memory allocation is performed, no special cleanup is required.
     */
    ~volume_controller();

    /**
     * Activates the volume control functionality.
     * This method sets the is_active flag to true, enabling the volume 
     * control functionality, allowing changes to the volume level to take effect.
     */
    void activate_volume();

    /**
     * Deactivates the volume control functionality.
     * This method sets the is_active flag to false, disabling any 
     * adjustments to the volume level.
     */
    void deactivate_volume();

    /**
     * Checks if the volume control is active.
     * This method checks the is_active flag to determine if volume control 
     * is enabled.
     *
     * @return true if volume control is active, false otherwise.
     */
    bool is_volume_active();

    /**
     * Increases the volume level by 1 dB.
     * Increases the volume level, up to a maximum of 10 dB, and recalculates 
     * the intensity of the audio signal. If the current volume is less than 
     * 10 dB, it is incremented by 1 dB.
     */
    void increase_volume();

    /**
     * Decreases the volume level by 1 dB.
     * Decreases the volume level, down to a minimum of 0 dB, and recalculates 
     * the intensity of the audio signal. If the current volume is greater than 
     * 0 dB, it is decremented by 1 dB.
     */
    void decrease_volume();

    /**
     * Gets the current volume intensity.
     * Retrieves the volume intensity as a float, representing the linear scale 
     * intensity corresponding to the current dB level.
     *
     * @return The current volume intensity as a float.
     */
    float get_volume_intesity();

    /**
     * Gets the current volume level in decibels (dB).
     * Returns a pointer to the internal volume_db variable. This allows other 
     * parts of the program to access or modify the current volume level in dB.
     *
     * @return A pointer to the volume level in dB.
     */
    int* get_volume_db();
};

#endif // _VOLUME_CONTROLLER_H