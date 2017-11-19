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

#include <iostream>
#include "GtkBase.h"

/* outside of a namespace */
extern "C" {

void appwindow1_destroy() {
	try {
		jape::GtkBase::jack_con_p->set_is_onair(false);

		if (!gtk_main_iteration())
			gtk_main_quit();
	} catch (...) {
		std::cerr << "Error: Failed to destroy a pedal window";
	}
}

void bpedal_click() {
	try {
		bool is_onair = jape::GtkBase::jack_con_p->invert_is_onair();

		gtk_switch_set_active(GTK_SWITCH(jape::GtkBase::switch_onair),
				      is_onair);
	} catch (...) {
		std::cerr << "Error: Failed to process a pedal press";
	}
}

void cbin_changed_cb() {
	try {
		gchar *new_port_name = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(jape::GtkBase::list_in));

		jape::GtkBase::jack_con_p->connect_new_to_in(
			std::string(new_port_name));

		g_free(new_port_name);
	} catch (...) {
		std::cerr << "Error: Failed to process an input reconnect";
	}
}

void cbout_changed_cb() {
	try {
		gchar *new_port_name = gtk_combo_box_text_get_active_text(
			GTK_COMBO_BOX_TEXT(jape::GtkBase::list_out));

		jape::GtkBase::jack_con_p->connect_out_to_new(
			std::string(new_port_name));

		g_free(new_port_name);
	} catch (...) {
		std::cerr << "Error: Failed to process an output reconnect";
	}
}

} /* extern "C" */
