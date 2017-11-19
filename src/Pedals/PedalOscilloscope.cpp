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

#include "PedalOscilloscope.h"

namespace jape {

struct PedalInfo PedalOscilloscope::info = {
	.client_name = "oscilloscope",
	.glade_path = "/layout/jape_oscilloscope.glade",
	.css_path = "/layout/jape_oscilloscope.css",
	.is_auto_connect = false,
	.n_ports_in = 1,
	.n_ports_out = 0,
};

void (*PedalOscilloscope::draw_cb)(float *, uint32_t) = nullptr;

struct PedalInfo PedalOscilloscope::get_info() const {
	return info;
}

int PedalOscilloscope::process(float *in, float *out, uint32_t n) const {
	(void)out;

	if (draw_cb) {
		draw_cb(in, n);
		return 0;
	}

	return 1;
}

void PedalOscilloscope::set_draw_cb(void (*cb)(float *, uint32_t)) const {
	draw_cb = cb;
}

} /* namespace jape */
