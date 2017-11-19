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

#ifndef SRC_AUDIOCON_JACKCON_H_
#define SRC_AUDIOCON_JACKCON_H_

#include <memory>
#include <set>
#include <stdexcept>
#include <vector>
#include <jack/jack.h>
#include "IPedal.h"

namespace jape {

struct JackConPortSelf {
	jack_port_t *jp = nullptr;
	std::string name_cur_con = "";
};

struct JackConPortsAll {
	uint8_t ports_num = 0;
	std::set<std::string> names_all = { };
	std::set<std::string> names_friends = { };
	std::vector <struct JackConPortSelf> self = { };
	JackConPortsAll() {
		resize(2);
	}
	JackConPortsAll(uint8_t n) {
		resize(n);
	}
	void resize(uint8_t n) {
		self.resize(n);
		ports_num = n;
	}
};

enum JackConPortDir {
	JACK_CON_PORT_IN,
	JACK_CON_PORT_OUT,
	JACK_CON_PORT_NUM,
};

class JackConException: public std::runtime_error {
public:
	JackConException(const char *m): std::runtime_error(m) { }
	JackConException(const std::string& m): std::runtime_error(m) { }
};

class JackCon {
private:
	static const IPedal *jape_p;
	jack_client_t *client;
	static std::array <struct JackConPortsAll, JACK_CON_PORT_NUM> ports;
	static bool is_onair;
	static std::vector<float> empty_buffer;
	static int jack_process_cb(jack_nframes_t nframes, void *arg);
	static void jack_shutdown_cb(void *arg);
	static void fill_port_names_vec(std::set<std::string>& names_vec,
		const char **names_ptr);
	static void fill_port_names_friends_vec(std::set<std::string>& dst,
		std::set<std::string>& src);
	void fill_port_names_all();
public:
	static const std::string PORT_NAME_IN;
	static const std::string PORT_NAME_OUT;
	JackCon(const IPedal& jape);
	virtual ~JackCon();
	void connect_new_to_in(std::string new_port_name) const;
	void connect_out_to_new(std::string new_port_name) const;
	std::set<std::string> get_port_names(enum JackConPortDir dir) const;
	bool get_is_onair() const;
	void set_is_onair(bool v) const;
	bool invert_is_onair() const;
};

} /* namespace jape */

#endif /* SRC_AUDIOCON_JACKCON_H_ */
