/*
 *  image.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 26/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WINGS_IMAGE_H_
#define _WINGS_IMAGE_H_

#include "basic.h"
#include "float4.h"
#include "color.h"

namespace wings
{
	struct Pixel {
		byte r, g, b, a;
		
		Pixel() : r(0), g(0), b(0), a(0) {}
		
		operator Color() const {
			return Color(r, g, b, a);
		}
	};
	
	class Image {
	private:
		Pixel* _data;
		size_t _w;
		size_t _h;
	public:
		Image();
		Image(size_t w, size_t h);
		Image(const Image& other);
		~Image();
		
		size_t width() const { return _w; }
		size_t height() const { return _h; }
		Pixel* pixels() const { return _data; }
		
		void chromify(const Color& color);
		void paste(const Image& source, size_t src_x, size_t src_y, size_t w, size_t h, size_t dst_x, size_t dst_y);
		
		static void load_png(Image& result, const String& path);
	};
}

#endif
