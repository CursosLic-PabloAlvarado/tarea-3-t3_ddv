/**
 * volume_controller.cpp
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

#include "volume_controller.h"
#include <cstring>
#include <iostream>
#include <math.h>

/**
 * Constructor for the volume_controller class.
 */
volume_controller::volume_controller() {
}

/**
 * Destructor for the volume_controller class.
 */
volume_controller::~volume_controller() {
}

/**
 * Activates the volume control functionality.
 * This sets the flag is_active to true, indicating that the volume controller
 * is actively managing volume changes.
 */
void volume_controller::activate_volume() {
    std::cout<<"volume activated"<<std::endl;
    this->volume_db = 1;
    this->volume_intesity = 1;
    this->is_active = true;
}

/**
 * Deactivates the volume control functionality.
 * This sets the flag is_active to false, disabling any volume adjustments.
 */
void volume_controller::deactivate_volume() {
    std::cout<<"Volume deactivated"<<std::endl;
    this->volume_db = 1;
    this->volume_intesity = 1;
    this->is_active = false;
}

/**
 * Checks whether the volume control is active.
 * @return true if the volume control is active, false otherwise.
 */
bool volume_controller::is_volume_active() {
    return this->is_active;
}

/**
 * Increases the volume level by 1 dB, up to a maximum of 10 dB.
 * After increasing the volume, the intensity is recalculated based on the new dB level.
 */
void volume_controller::increase_volume() {
    if (this->volume_db < 10) {
        this->volume_db++;
        this->db_conversion();  // Recompute intensity based on new dB level
    }
}

/**
 * Decreases the volume level by 1 dB, down to a minimum of 0 dB.
 * After decreasing the volume, the intensity is recalculated based on the new dB level.
 */
void volume_controller::decrease_volume() {
    if (this->volume_db > 0) {
        this->volume_db--;
        this->db_conversion();  // Recompute intensity based on new dB level
    }
}

/**
 * Retrieves the current volume intensity.
 * @return The current volume intensity as a float.
 */
float volume_controller::get_volume_intesity() {
    return this->volume_intesity;
}

/**
 * Converts the volume level from decibels (dB) to intensity.
 * If the volume level is 0 dB, the intensity is set to 0 (no sound).
 */
void volume_controller::db_conversion() {
    if (this->volume_db == 0) {
        this->volume_intesity = 0;  // No intensity when volume is 0 dB
    } else {
        // Define the dB range for conversion
        double dB_min = -10.0;
        double dB_max = 70.0;

        // Compute the factor for dB to intensity conversion using a scaling factor
        double factor_dB = ((this->volume_db / 10.0) * (dB_max - dB_min)) + dB_min;

        // Convert from dB to intensity (linear scale)
        this->volume_intesity = pow(10, factor_dB / 20);
    }
}

/**
 * Retrieves the current volume level in decibels (dB).
 * @return A pointer to the current volume level in dB.
 */
int* volume_controller::get_volume_db() {
    return &(this->volume_db);
}