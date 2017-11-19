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

#include "GtkBase.h"

namespace jape {

const std::string GtkBase::MAIN_WINDOW_NAME = "applicationwindow1";
const std::string GtkBase::SWITCH_ON_AIR_NAME = "sonair";
const std::string GtkBase::COMBOBOX_IN_NAME = "cbin";
const std::string GtkBase::COMBOBOX_OUT_NAME = "cbout";
const IPedal *GtkBase::jape_p = nullptr;
const JackCon *GtkBase::jack_con_p = nullptr;
GtkWidget *GtkBase::window = nullptr;
GtkWidget *GtkBase::switch_onair = nullptr;
GtkWidget *GtkBase::list_in = nullptr;
GtkWidget *GtkBase::list_out = nullptr;
GtkBuilder *GtkBase::builder = nullptr;

GtkBase::GtkBase(const IPedal& jape, const JackCon& jack_con) {
	/* static pointer to the pedal */
	GtkBase::jape_p = &jape;
	struct PedalInfo info = jape.get_info();

	/* static pointer to the JackCon */
	GtkBase::jack_con_p = &jack_con;

	gtk_init(nullptr, nullptr);

	/* layout builder */
	builder = gtk_builder_new();
	if (!builder)
		throw GtkBaseException("Failed to create a GUI builder");

	if (!gtk_builder_add_from_resource(builder, info.glade_path.c_str(),
				  nullptr))
		throw GtkBaseException(
			"Failed to load a layout from resources");

	window = GTK_WIDGET(gtk_builder_get_object(builder,
						   MAIN_WINDOW_NAME.c_str()));
	if (!window)
		throw GtkBaseException("Failed to get a widget: " +
					MAIN_WINDOW_NAME);

	gtk_builder_connect_signals(builder, nullptr);

	/* style provider */
	GtkCssProvider *style_provider = gtk_css_provider_new();
	if (!style_provider)
		throw GtkBaseException("Failed to create a style provider");

	GdkDisplay *display = gdk_display_get_default();
	if (!display)
		throw GtkBaseException("Failed to get a display object");

	GdkScreen *screen = gdk_display_get_default_screen(display);
	if (!screen)
		throw GtkBaseException("Failed to get a screen object");

	gtk_style_context_add_provider_for_screen(screen,
		GTK_STYLE_PROVIDER(style_provider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

	GBytes *css_bytes = g_resources_lookup_data(info.css_path.c_str(),
				G_RESOURCE_LOOKUP_FLAGS_NONE,
				nullptr);
	if (!css_bytes)
		GtkBaseException("Failed to lookup a style in resources");

	gsize css_length;
	const gchar *css_data = (const gchar *)g_bytes_get_data(css_bytes,
								&css_length);
	if (!css_data)
		GtkBaseException("Failed to get style data");

	if (!gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(style_provider),
					     css_data, css_length, nullptr))
		throw GtkBaseException("Failed to load a style from data");

	g_bytes_unref(css_bytes);
	g_object_unref(style_provider);

	/* get pointers to widgets */
	switch_onair = GTK_WIDGET(gtk_builder_get_object(builder,
					SWITCH_ON_AIR_NAME.c_str()));
	if (!switch_onair)
		throw GtkBaseException("Failed to get a widget: " +
					SWITCH_ON_AIR_NAME);

	gtk_switch_set_active(GTK_SWITCH(switch_onair),
			      jack_con.get_is_onair());

	list_in = GTK_WIDGET(gtk_builder_get_object(builder,
					COMBOBOX_IN_NAME.c_str()));
	if (!list_in)
		throw GtkBaseException("Failed to get a widget: " +
					COMBOBOX_IN_NAME);

	list_out = GTK_WIDGET(gtk_builder_get_object(builder,
					COMBOBOX_OUT_NAME.c_str()));
	if (!list_out)
		throw GtkBaseException("Failed to get a widget: " +
					COMBOBOX_OUT_NAME);

	/* fill combobox lists with possiible connectors (reversed) */
	fill_list(list_in, jack_con.get_port_names(JACK_CON_PORT_OUT));
	fill_list(list_out, jack_con.get_port_names(JACK_CON_PORT_IN));
}

GtkBase::~GtkBase() {
	if (!gtk_main_iteration())
		gtk_main_quit();

	g_object_unref(builder);
}

void GtkBase::gui_start() {
	gtk_widget_show(window);

	gtk_main();
}

void GtkBase::fill_list(GtkWidget *list,
			const std::set<std::string>& vals) {
	GtkComboBoxText *cbox = GTK_COMBO_BOX_TEXT(list);
	if (!cbox)
		throw GtkBaseException("Failed to get a widget to fill");

	/* add an empty item */
	gtk_combo_box_text_append(cbox, "", "");

	/* add others */
	for (auto it = vals.begin(); it != vals.end(); ++it) {
		std::string short_name(*it, 0, 4);

		gtk_combo_box_text_append(cbox, it->c_str(),
					  short_name.c_str());
	}

	struct PedalInfo info = jape_p->get_info();

	/* select the first non-empty */
	if (info.is_auto_connect && vals.size())
		gtk_combo_box_set_active(GTK_COMBO_BOX(cbox), 1);
}

} /* namespace jape */
