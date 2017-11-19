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

#ifndef SRC_GUI_GTKDISTORTION_H_
#define SRC_GUI_GTKDISTORTION_H_

#include "GtkBase.h"
#include "PedalDistortion.h"

namespace jape {

class GtkDistortion: public GtkBase {
private:
	static const std::string SCALE_LEVEL_NAME;
	static const std::string SCALE_GAIN_NAME;
public:
	static const PedalDistortion *jape_dist_p;
	static GtkWidget *scale_level;
	static GtkWidget *scale_gain;
	GtkDistortion(const IPedal& jape, const JackCon& jack_con);
	virtual ~GtkDistortion();
};

} /* namespace jape */

#endif /* SRC_GUI_GTKDISTORTION_H_ */
