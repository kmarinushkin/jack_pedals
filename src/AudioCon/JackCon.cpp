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

#include <cstring>
#include <iostream>
#include "JackCon.h"

namespace jape {

const IPedal *JackCon::jape_p = nullptr;
const std::string JackCon::PORT_NAME_IN = "in";
const std::string JackCon::PORT_NAME_OUT = "out";
std::array <struct JackConPortsAll, JACK_CON_PORT_NUM> JackCon::ports;
bool JackCon::is_onair = false;
std::vector<float> JackCon::empty_buffer = { };

JackCon::JackCon(const IPedal& jape): client(nullptr) {
	jack_status_t status;

	/* static pointer to the pedal */
	JackCon::jape_p = &jape;
	struct PedalInfo info = jape.get_info();

	/* open a client connection to the Jack server */
	client = jack_client_open(info.client_name.c_str(),
				  JackNullOption, &status, nullptr);
	if (!client)
		throw JackConException("Failed to open client, status = " +
					std::to_string(status));

	/* get number of ports */
	ports[JACK_CON_PORT_IN].resize(info.n_ports_in);
	ports[JACK_CON_PORT_OUT].resize(info.n_ports_out);

	/* register ports */
	uint8_t c = 0;
	for (auto it = ports[JACK_CON_PORT_IN].self.begin();
		it != ports[JACK_CON_PORT_IN].self.end(); ++it) {
		std::string portname = PORT_NAME_IN + std::to_string(++c);

		it->jp = jack_port_register(client, portname.c_str(),
					    JACK_DEFAULT_AUDIO_TYPE,
					    JackPortIsInput, 0);
		if (!it->jp)
			throw JackConException("Failed to register in-port");
	}

	c = 0;
	for (auto it = ports[JACK_CON_PORT_OUT].self.begin();
		it != ports[JACK_CON_PORT_OUT].self.end(); ++it) {
		std::string portname = PORT_NAME_OUT + std::to_string(++c);

		it->jp = jack_port_register(client, portname.c_str(),
					    JACK_DEFAULT_AUDIO_TYPE,
					    JackPortIsOutput, 0);
		if (!it->jp)
			throw JackConException("Failed to register out-port");
	}

	/* attach callback functions */
	if(jack_set_process_callback(client, jack_process_cb, &ports))
		throw JackConException("Failed to attach process callback");

	jack_on_shutdown(client, jack_shutdown_cb, nullptr);

	/* client is ready, start */
	if (jack_activate(client))
		throw JackConException("Failed to activate client");

	/* fill the names of all existing ports */
	fill_port_names_all();
}

JackCon::~JackCon() {
	for (auto it = ports[JACK_CON_PORT_IN].self.begin();
		it != ports[JACK_CON_PORT_IN].self.end(); ++it) {
		jack_port_unregister(client, it->jp);
	}

	for (auto it = ports[JACK_CON_PORT_OUT].self.begin();
		it != ports[JACK_CON_PORT_OUT].self.end(); ++it) {
		jack_port_unregister(client, it->jp);
	}

	jack_deactivate(client);
	jack_client_close(client);
}

int JackCon::jack_process_cb(jack_nframes_t nframes, void *arg) {
	try {
		int rc = 0;
		std::array <struct JackConPortsAll, JACK_CON_PORT_NUM> *p =
			(std::array <struct JackConPortsAll,
				     JACK_CON_PORT_NUM> *)arg;

		if (empty_buffer.size() < nframes)
			empty_buffer.resize(nframes);

		uint8_t ports_num_max = std::max(
					(*p)[JACK_CON_PORT_IN].ports_num,
					(*p)[JACK_CON_PORT_OUT].ports_num);
		for (uint8_t i = 0; i < ports_num_max; i++) {
			float *in;
			float *out;

			if (i < (*p)[JACK_CON_PORT_IN].ports_num)
				in = (float *)jack_port_get_buffer(
					(*p)[JACK_CON_PORT_IN].self[i].jp,
					nframes);
			else
				in = &empty_buffer[0];

			if (i < (*p)[JACK_CON_PORT_OUT].ports_num)
				out = (float *)jack_port_get_buffer(
					(*p)[JACK_CON_PORT_OUT].self[i].jp,
					nframes);
			else
				out = &empty_buffer[0];

			if (!is_onair)
				/* copy signal without modifications */
				(void)std::memcpy(out, in,
					sizeof(jack_default_audio_sample_t) *
					nframes);
				/* else apply the pedal processing */
			else if ((rc = jape_p->process(in, out,
						      (uint32_t)nframes)))
				break;
		}

		return rc;
	} catch (const std::exception& e) {
		std::cerr << "Error: JackCon processing: " << e.what();
	} catch (...) {
		std::cerr << "Error: JackCon processing";
	}

	return 1;
}

void JackCon::jack_shutdown_cb(void *arg) {
	(void)arg;
	/* Jack server is shut down, ignore */
}

void JackCon::fill_port_names_vec(std::set<std::string>& names_vec,
				  const char **names_ptr) {
	names_vec.clear();

	if (names_ptr) {
		while (names_ptr[0]) {
			names_vec.insert(std::string(names_ptr[0]));
			names_ptr++;
		}
	}
}

void JackCon::fill_port_names_friends_vec(std::set<std::string>& dst,
	std::set<std::string>& src) {
	const std::string sep = ":";

	dst.clear();

	for (auto it = src.begin(); it != src.end(); ++it) {
		/* get only the client name */
		std::string cn(*it, 0, it->find(sep, 0));
		struct PedalInfo info = jape_p->get_info();

		if (!cn.empty() && cn != info.client_name)
			dst.insert(cn);
	}
}

void JackCon::fill_port_names_all() {
	const char **names_all = jack_get_ports(client, nullptr, nullptr,
						JackPortIsInput);

	fill_port_names_vec(ports[JACK_CON_PORT_IN].names_all, names_all);
	fill_port_names_friends_vec(ports[JACK_CON_PORT_IN].names_friends,
				    ports[JACK_CON_PORT_IN].names_all);

	jack_free(names_all);

	names_all = jack_get_ports(client, nullptr, nullptr,
				   JackPortIsOutput);

	fill_port_names_vec(ports[JACK_CON_PORT_OUT].names_all, names_all);
	fill_port_names_friends_vec(ports[JACK_CON_PORT_OUT].names_friends,
				    ports[JACK_CON_PORT_OUT].names_all);

	jack_free(names_all);
}

void JackCon::connect_new_to_in(std::string new_port_name) const {
	/* disconnect from where currently connected */
	for (uint8_t i = 0; i< ports[JACK_CON_PORT_IN].ports_num; i++) {
		struct JackConPortSelf *self_in =
			&ports[JACK_CON_PORT_IN].self[i];

		if (!self_in->name_cur_con.empty()) {
			if (jack_disconnect(client,
					    self_in->name_cur_con.c_str(),
					    jack_port_name(self_in->jp))) {
				/* not an exception, can continue */
				std::cerr << "Warning: JackCon: " <<
					"cannot disconnect an input port\n";

				continue;
			}

			self_in->name_cur_con = "";
		}
	}

	if (new_port_name.empty())
		return;

	/* get a full port-name from a selected short port-name */
	uint8_t c = 0;
	for (auto it = ports[JACK_CON_PORT_OUT].names_all.begin();
		it != ports[JACK_CON_PORT_OUT].names_all.end(); ++it) {
		struct JackConPortSelf *self_in =
			&ports[JACK_CON_PORT_IN].self[c];

		/* if the current name starts with the requested shortname */
		if (it->find(new_port_name) == 0) {
			if (jack_connect(client, it->c_str(),
					 jack_port_name(self_in->jp))) {
				/* not an exception, can continue */
				std::cerr << "Warning: JackCon: " <<
					"cannot connect an input port\n";

				continue;
			}

			self_in->name_cur_con = *it;
			c++;
			if (c >= ports[JACK_CON_PORT_IN].ports_num)
				break;
		}
	}
}

void JackCon::connect_out_to_new(std::string new_port_name) const {
	/* disconnect from where currently connected */
	for (uint8_t i = 0; i< ports[JACK_CON_PORT_OUT].ports_num; i++) {
		struct JackConPortSelf *self_out =
			&ports[JACK_CON_PORT_OUT].self[i];

		if (!self_out->name_cur_con.empty()) {
			if (jack_disconnect(client,
					    jack_port_name(self_out->jp),
					    self_out->name_cur_con.c_str())) {
				/* not an exception, can continue */
				std::cerr << "Warning: JackCon: " <<
					"cannot disconnect an output port\n";

				continue;
			}

			self_out->name_cur_con = "";
		}
	}

	if (new_port_name.empty())
		return;

	/* get a full port-name from a selected short port-name */
	uint8_t c = 0;
	for (auto it = ports[JACK_CON_PORT_IN].names_all.begin();
		it != ports[JACK_CON_PORT_IN].names_all.end(); ++it) {
		struct JackConPortSelf *self_out =
			&ports[JACK_CON_PORT_OUT].self[c];

		/* if the current name starts with the requested shortname */
		if (it->find(new_port_name) == 0) {
			if (jack_connect(client, jack_port_name(self_out->jp),
					 it->c_str())) {
				/* not an exception, can continue */
				std::cerr << "Warning: JackCon: " <<
					"cannot connect an output port\n";

				continue;
			}

			self_out->name_cur_con = *it;
			c++;
			if (c >= ports[JACK_CON_PORT_OUT].ports_num)
				break;
		}
	}
}

std::set<std::string> JackCon::get_port_names(
		enum JackConPortDir dir) const {
	return ports[dir].names_friends;
}

bool JackCon::get_is_onair() const {
	return is_onair;
}

void JackCon::set_is_onair(bool v) const {
	is_onair = v;
}

bool JackCon::invert_is_onair() const {
	is_onair = !is_onair;

	return is_onair;
}

} /* namespace jape */
