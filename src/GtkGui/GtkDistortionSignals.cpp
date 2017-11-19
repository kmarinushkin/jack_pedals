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
#include "GtkDistortion.h"

/* outside of a namespace */
extern "C" {

gboolean scale_level_change_value_cb(GtkRange *range, GtkScrollType scroll,
				 gdouble value, gpointer user_data) {
	(void)range;
	(void)scroll;
	(void)user_data;

	try {
		jape::GtkDistortion::jape_dist_p->set_level(value);
	} catch (...) {
		std::cerr << "Error: Failed to process a level change";
	}

	return FALSE;
}

gboolean scale_gain_change_value_cb(GtkRange *range, GtkScrollType scroll,
				 gdouble value, gpointer user_data) {
	(void)range;
	(void)scroll;
	(void)user_data;

	try {
		jape::GtkDistortion::jape_dist_p->set_gain(value);
	} catch (...) {
		std::cerr << "Error: Failed to process a gain change";
	}

	return FALSE;
}

} /* extern "C" */

