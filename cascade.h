#ifndef _CASCADE_H
#define _CASCADE_H

#include <iostream>
#include <array>
#include <vector>
#include "jack_client.h"
#include "volume_controller.h"
#include "biquad.h"

/**
 * Jack client class template
 *
 * This class wraps basic JACK functionality and provides cascaded biquad filters.
 *
 * @tparam N Number of cascaded biquad filters
 */
template <unsigned int N>
class cascade : public jack::client {

private:
  /**
   * Pointer to the volume controller. q
   */
  volume_controller* volume_controller_prt;

  std::vector<biquad> biquad_filters;

public:

  cascade();

  /**
   * Process method override.
   * Handles audio processing through the cascaded biquad filters.
   */
    virtual bool process(jack_nframes_t nframes,
                        const sample_t *const in,
                        sample_t *const out) override;

  /**
   * Set filter coefficients for all biquad filters in the cascade.
   */
  void set_coefficients(const std::vector<std::vector<sample_t>>& coefficients);
  void set_volume(volume_controller* volume_ptr);
};
#endif