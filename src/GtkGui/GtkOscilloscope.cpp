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
#include "GtkOscilloscope.h"
#include "PedalOscilloscope.h"

namespace jape {

const std::string GtkOscilloscope::DRAWING_AREA_PLOT_NAME = "daplot";
const uint32_t GtkOscilloscope::NEXT_DRAW_PERIOD = 1000;
GtkWidget *GtkOscilloscope::plot_area = nullptr;
GMutex GtkOscilloscope::plot_vals_mut = { };
std::vector<float> GtkOscilloscope::plot_vals;
clock_t GtkOscilloscope::next_draw_time = 0;
gulong GtkOscilloscope::daplot_draw_handler = 0;
guint GtkOscilloscope::width = 0;
guint GtkOscilloscope::height = 0;

GtkOscilloscope::GtkOscilloscope(const IPedal& jape, const JackCon& jack_con):
	GtkBase(jape, jack_con) {
	/* get pointers to widgets */
	plot_area = GTK_WIDGET(gtk_builder_get_object(builder,
				DRAWING_AREA_PLOT_NAME.c_str()));
	if (!plot_area)
		throw GtkBaseException("Failed to get a widget: " +
					DRAWING_AREA_PLOT_NAME);

	/* set a callback for a pedal */
	const PedalOscilloscope *jape_oscil_p =
		(const PedalOscilloscope *)jape_p;
	jape_oscil_p->set_draw_cb(&draw_plot);

	/* set a callback for a drawing area draw */
	daplot_draw_handler = g_signal_connect(plot_area, "draw",
			(GCallback)&GtkOscilloscope::daplot_draw_cb,
			nullptr);

	width = gtk_widget_get_allocated_width(plot_area);
	height = gtk_widget_get_allocated_height(plot_area);

	/* init the lock */
	g_mutex_init(&plot_vals_mut);

	next_draw_time = clock();
}

GtkOscilloscope::~GtkOscilloscope() {
	lock();

	/* g_signal_handler_disconnect is executed during window destroy */

	const PedalOscilloscope *jape_oscil_p =
		(const PedalOscilloscope *)jape_p;
	jape_oscil_p->set_draw_cb(nullptr);

	unlock();

	g_mutex_clear(&plot_vals_mut);
}

gboolean GtkOscilloscope::daplot_draw_cb(GtkWidget *widget, cairo_t *cr,
					 gpointer data) {
	(void)data;

	try {
		double scale_x = 2;
		double scale_y = 80;
		GdkRGBA color;
		width = gtk_widget_get_allocated_width(widget);
		height = gtk_widget_get_allocated_height(widget);
		double p0_y = height / 2;
		GtkStyleContext *context =
			gtk_widget_get_style_context(widget);

		if (!context)
			throw jape::GtkBaseException(
				"Failed to get a style context");

		gtk_render_background(context, cr, 0, 0, width, height);

		cairo_new_path(cr);

		lock();

		try {
			for (uint32_t i = 0;
				i < plot_vals.size() &&
				i < (width / scale_x) + 1; i++) {
				double x = i * scale_x;
				double y = p0_y + (plot_vals[i] * scale_y);

				if (!x)
					cairo_move_to(cr, x, y);
				else
					cairo_line_to(cr, x, y);
			}

			unlock();
		} catch (...) {
			unlock();
			throw;
		}

		gtk_style_context_get_color(context,
			gtk_style_context_get_state(context),
			&color);

		gdk_cairo_set_source_rgba(cr, &color);

		cairo_stroke(cr);

		/* unpause input signal processing after drawing */
		next_draw_time = clock() + NEXT_DRAW_PERIOD;
	} catch (...) {
		std::cerr << "Error: Failed to process a plot draw";
	}

	return FALSE;
}

void GtkOscilloscope::draw_plot(float *vals, uint32_t n) {
	if (!next_draw_time || next_draw_time > clock())
		return;

	bool need_redraw = false;

	lock();

	try {
		if (plot_vals.size() < n)
			plot_vals.resize(n);

		for (uint32_t i = 0; i < n; i++) {
			if (plot_vals[i] != vals[i])
				need_redraw = true;

			plot_vals[i] = vals[i];
		}

		unlock();
	} catch (...) {
		unlock();
		throw;
	}

	if (need_redraw) {
		/* pause input signal processing while drawing */
		next_draw_time = 0;

		gtk_widget_queue_draw_area(plot_area, 0, 0, width, height);
	}
}

void GtkOscilloscope::lock() {
	g_mutex_lock(&plot_vals_mut);
}

void GtkOscilloscope::unlock() {
	g_mutex_unlock(&plot_vals_mut);
}

} /* namespace jape */
