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

#ifndef SRC_GTKGUI_GTKBASE_H_
#define SRC_GTKGUI_GTKBASE_H_

#include <stdexcept>
#include <gtk/gtk.h>
#include "IPedal.h"
#include "JackCon.h"

namespace jape {

class GtkBaseException: public std::runtime_error {
public:
	GtkBaseException(const char *m): std::runtime_error(m) { }
	GtkBaseException(const std::string& m): std::runtime_error(m) { }
};

class GtkBase {
protected:
	static const std::string MAIN_WINDOW_NAME;
	static const std::string SWITCH_ON_AIR_NAME;
	static const std::string COMBOBOX_IN_NAME;
	static const std::string COMBOBOX_OUT_NAME;
	static GtkWidget *window;
	static GtkBuilder *builder;
	void fill_list(GtkWidget *list, const std::set<std::string>& vals);
public:
	static const JackCon *jack_con_p;
	static const IPedal *jape_p;
	static GtkWidget *list_in;
	static GtkWidget *list_out;
	static GtkWidget *switch_onair;
	GtkBase(const IPedal& jape, const JackCon& jack_con);
	virtual ~GtkBase();
	void gui_start();
};

} /* namespace jape */

#endif /* SRC_GTKGUI_GTKBASE_H_ */
