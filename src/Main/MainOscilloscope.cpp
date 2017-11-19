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
#include "config.h"
#include "JackCon.h"
#include "GtkOscilloscope.h"
#include "PedalOscilloscope.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	try {
		jape::PedalOscilloscope jape_main;
		jape::JackCon jc(jape_main);
		jape::GtkOscilloscope gui(jape_main, jc);

		gui.gui_start();

		return 0;
	} catch (const jape::JackConException& e) {
		std::cerr << "Error: JackCon: " << e.what();
	} catch (const jape::GtkBaseException& e) {
		std::cerr << "Error: GtkBase: " << e.what();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what();
	} catch (...) {
		std::cerr << "Error: Unknown";
	}

	return 1;
}
