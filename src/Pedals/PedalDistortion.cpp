/**
 * Copyright (C) 2017 Kirill Marinushkin <k.marinushkin@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include <cmath>
#include <stdexcept>
#include "PedalDistortion.h"

namespace jape {

struct PedalInfo PedalDistortion::info = {
	.client_name = "distortion",
	.glade_path = "/layout/jape_distortion.glade",
	.css_path = "/layout/jape_distortion.css",
	.is_auto_connect = true,
	.n_ports_in = 2,
	.n_ports_out = 2,
};

double PedalDistortion::level = 0;
double PedalDistortion::gain = 0;

struct PedalInfo PedalDistortion::get_info() const {
	return info;
}

int PedalDistortion::process(float *in, float *out, uint32_t n) const {
	const float g = gain;
	const float lvl = level;
	const float lvl_low = lvl / g;

	for (uint32_t i = 0 ; i < n; i++) {
		float abs_x = std::abs(in[i]);
		float sign = (in[i] >= 0 ? 1 : -1);

		if (abs_x < lvl_low) {
			/* amplify */
			out[i] = in[i] * g;
		} else {
			/* cut */
			out[i] = sign * lvl;
		}
	}

	return 0;
}

void PedalDistortion::set_level(double v) const {
	level = v;
}

void PedalDistortion::set_gain(double v) const {
	gain = v;
}

} /* namespace jape */
