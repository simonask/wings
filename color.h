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

typedef struct {
	float r, g, b, a;
} Color;

static const Color COLOR_WHITE  = { 1.0f, 1.0f, 1.0f, 1.0f };
static const Color COLOR_BLUE   = { 0.0f, 0.0f, 1.0f, 1.0f };
static const Color COLOR_GREEN  = { 0.0f, 1.0f, 0.0f, 1.0f };
static const Color COLOR_RED    = { 1.0f, 0.0f, 0.0f, 1.0f };
static const Color COLOR_YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };


#endif // _COLOR_H_
