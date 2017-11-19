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

#ifndef SRC_PEDALS_IPEDAL_H_
#define SRC_PEDALS_IPEDAL_H_

#include <string>

namespace jape {

struct PedalInfo {
	std::string client_name;
	std::string glade_path;
	std::string css_path;
	bool is_auto_connect;
	uint32_t n_ports_in;
	uint32_t n_ports_out;
};

class IPedal {
public:
	virtual struct PedalInfo get_info() const = 0;
	virtual int process(float *in, float *out, uint32_t n) const = 0;
};

} /* namespace jape */

#endif /* SRC_PEDALS_IPEDAL_H_ */
