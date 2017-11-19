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

#ifndef SRC_PEDALS_PEDALDISTORTION_H_
#define SRC_PEDALS_PEDALDISTORTION_H_

#include "IPedal.h"

namespace jape {

class PedalDistortion: public IPedal {
private:
	static struct PedalInfo info;
	static double level;
	static double gain;
public:
	struct PedalInfo get_info() const;
	int process(float *in, float *out, uint32_t n) const;
	void set_level(double v) const;
	void set_gain(double v) const;
};

} /* namespace jape */

#endif /* SRC_PEDALS_PEDALDISTORTION_H_ */
