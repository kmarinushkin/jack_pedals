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

#ifndef SRC_GUI_GTKGENERATOR_H_
#define SRC_GUI_GTKGENERATOR_H_

#include "GtkBase.h"

namespace jape {

class GtkGenerator: public GtkBase {
public:
	GtkGenerator(const IPedal& jape, const JackCon& jack_con);
	virtual ~GtkGenerator();
};

} /* namespace jape */

#endif /* SRC_GUI_GTKGENERATOR_H_ */
