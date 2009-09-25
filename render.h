/*
 *  render.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 12/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RENDER_H_
#define _RENDER_H_

#include "float4.h"
#include "color.h"
namespace wings
{
typedef void(*RenderFunc)(void* userdata);
typedef void(*RenderGetPositionFunc)(void* userdata, float4s& out); // 3D position (x, y, z, _), used for depth-sorting
typedef void(*RenderGetBoundingBoxFunc)(void* userdata, float4s& out); // 2D bounding box (x, y, w, h)

void render_text(const char* text, float x, float y, Color color);
}

#endif // _RENDER_H_
