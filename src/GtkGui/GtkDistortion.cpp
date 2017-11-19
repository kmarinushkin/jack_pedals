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

#include "GtkDistortion.h"

namespace jape {

const std::string GtkDistortion::SCALE_LEVEL_NAME = "scale_level";
const std::string GtkDistortion::SCALE_GAIN_NAME = "scale_gain";
const PedalDistortion *GtkDistortion::jape_dist_p = nullptr;
GtkWidget *GtkDistortion::scale_level = nullptr;
GtkWidget *GtkDistortion::scale_gain = nullptr;

GtkDistortion::GtkDistortion(const IPedal& jape, const JackCon& jack_con):
	GtkBase(jape, jack_con) {
	jape_dist_p = (const PedalDistortion *)jape_p;

	/* get pointers to widgets */
	scale_level = GTK_WIDGET(gtk_builder_get_object(builder,
				SCALE_LEVEL_NAME.c_str()));
	if (!scale_level)
		throw GtkBaseException("Failed to get a widget: " +
					SCALE_LEVEL_NAME);

	scale_gain = GTK_WIDGET(gtk_builder_get_object(builder,
				SCALE_GAIN_NAME.c_str()));
	if (!scale_gain)
		throw GtkBaseException("Failed to get a widget: " +
					SCALE_GAIN_NAME);

	/* set initial values */
	const PedalDistortion *jape_p =
		(const PedalDistortion *)GtkBase::jape_p;

	gdouble value = gtk_range_get_value(GTK_RANGE(scale_level));
	jape_p->set_level(value);

	value = gtk_range_get_value(GTK_RANGE(scale_gain));
	jape_p->set_gain(value);
}

GtkDistortion::~GtkDistortion() {
}

} /* namespace jape */
