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

#include "GtkGenerator.h"

namespace jape {

GtkGenerator::GtkGenerator(const IPedal& jape, const JackCon& jack_con):
	GtkBase(jape, jack_con) {
}

GtkGenerator::~GtkGenerator() {
}

} /* namespace jape */
