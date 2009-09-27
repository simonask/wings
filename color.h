/*
 *  color.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 13/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include "float4.h"

namespace wings {

struct Color {
	float r, g, b, a;
	
	Color(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}
	Color(byte _r, byte _g, byte _b, byte _a = 255) : r((float)_r/255), g((float)_g/255), b((float)_b/255), a((float)_a/255) {}
	
	Color& operator=(const Color& c) {
		r = c.r; g = c.g; b = c.b; a = c.a;
		return *this;
	}
};

static const Color COLOR_BLACK    ( 0.0f, 0.0f, 0.0f, 1.0f );
static const Color COLOR_WHITE    ( 1.0f, 1.0f, 1.0f, 1.0f );
static const Color COLOR_BLUE     ( 0.0f, 0.0f, 1.0f, 1.0f );
static const Color COLOR_GREEN    ( 0.0f, 1.0f, 0.0f, 1.0f );
static const Color COLOR_RED      ( 1.0f, 0.0f, 0.0f, 1.0f );
static const Color COLOR_YELLOW   ( 1.0f, 1.0f, 0.0f, 1.0f );
}

#endif // _COLOR_H_
