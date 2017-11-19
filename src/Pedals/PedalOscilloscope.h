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

#ifndef SRC_PEDALS_PEDALOSCILLOSCOPE_H_
#define SRC_PEDALS_PEDALOSCILLOSCOPE_H_

#include "IPedal.h"

namespace jape {

class PedalOscilloscope: public IPedal {
private:
	static struct PedalInfo info;
	static void (*draw_cb)(float *, uint32_t);
public:
	struct PedalInfo get_info() const;
	int process(float *in, float *out, uint32_t n) const;
	void set_draw_cb(void (*cb)(float *, uint32_t)) const;
};

} /* namespace jape */

#endif /* SRC_PEDALS_PEDALOSCILLOSCOPE_H_ */
