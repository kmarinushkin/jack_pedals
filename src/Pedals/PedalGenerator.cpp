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
#include "PedalGenerator.h"

namespace jape {

struct PedalSinTable PedalGenerator::sin_table = {
	.SIZE = 64,
	.cur = 0,
	.table = std::vector<float>(),
};

struct PedalInfo PedalGenerator::info = {
	.client_name = "generator",
	.glade_path = "/layout/jape_generator.glade",
	.css_path = "/layout/jape_generator.css",
	.is_auto_connect = false,
	.n_ports_in = 0,
	.n_ports_out = 1,
};

struct PedalInfo PedalGenerator::get_info() const {
	return info;
}

int PedalGenerator::process(float *in, float *out, uint32_t n) const {
	(void)in;

	/* first time - init the table */
	if (sin_table.table.empty()) {
		const double scale = 1;

		for (uint32_t i = 0; i < sin_table.SIZE; i++) {
			sin_table.table.push_back(
				(float)std::sin(
					((double)i/(double)sin_table.SIZE) *
					M_PI * 2.0) * scale);
		}

		sin_table.cur = 0;
	}

	/* fill output from the table */
	for (uint32_t i = 0; i < n; i++) {
		out[i] = sin_table.table[sin_table.cur];

		if (++sin_table.cur >= sin_table.table.size())
			sin_table.cur = 0;
	}

	return 0;
}

} /* namespace jape */
