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

#ifndef SRC_PEDALS_PEDALGENERATOR_H_
#define SRC_PEDALS_PEDALGENERATOR_H_

#include <vector>
#include "IPedal.h"

namespace jape {

struct PedalSinTable {
	uint32_t SIZE;
	uint32_t cur;
	std::vector<float> table;
};

class PedalGenerator: public IPedal {
private:
	static struct PedalSinTable sin_table;
	static struct PedalInfo info;
public:
	std::string get_client_name() const;
	std::string get_glade_path() const;
	std::string get_css_path() const;
	bool is_auto_connect() const;
	struct PedalInfo get_info() const;
	int process(float *in, float *out, uint32_t n) const;
};

} /* namespace jape */

#endif /* SRC_PEDALS_PEDALGENERATOR_H_ */
