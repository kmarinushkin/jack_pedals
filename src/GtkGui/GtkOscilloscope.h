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

#ifndef SRC_GUI_GTKOSCILLOSCOPE_H_
#define SRC_GUI_GTKOSCILLOSCOPE_H_

#include <ctime>
#include <vector>
#include "GtkBase.h"

namespace jape {

class GtkOscilloscope: public GtkBase {
private:
	static const std::string DRAWING_AREA_PLOT_NAME;
	static const uint32_t NEXT_DRAW_PERIOD;
	static GtkWidget *plot_area;
	static std::vector<float> plot_vals;
	static GMutex plot_vals_mut;
	static clock_t next_draw_time;
	static gulong daplot_draw_handler;
	static guint width;
	static guint height;
	static gboolean daplot_draw_cb(GtkWidget *widget, cairo_t *cr,
					gpointer data);
public:
	GtkOscilloscope(const IPedal& jape, const JackCon& jack_con);
	virtual ~GtkOscilloscope();
	static void draw_plot(float *vals, uint32_t n);
	static void lock();
	static void unlock();
};

} /* namespace jape */

#endif /* SRC_GUI_GTKOSCILLOSCOPE_H_ */
