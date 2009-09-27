/*
 *  font.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 27/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _WINGS_FONT_H_
#define _WINGS_FONT_H_

#include "image.h"

namespace wings {
	class Font {
	private:
		Image _faces;
		byte _char_width;
		byte _char_height;
	public:
		Font(const Image& faces, byte cw, byte ch) : _faces(faces), _char_width(cw), _char_height(ch) {}
		
		byte char_width() const { return _char_width; }
		byte char_height() const { return _char_height; }
		
		void render(Image& dst, size_t dst_x, size_t dst_y, const char* str, const Color& color = COLOR_BLACK);
		
		void required_size_for_text(const char* str, uint32_t& w, uint32_t& h);
	};
}

#endif
