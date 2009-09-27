/*
 *  font.cpp
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 27/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "font.h"

namespace wings {
	
	void Font::render(Image& dst, size_t dst_x, size_t dst_y, const char* str, const Color& color) {
		// TODO: Do something for incremental rendering that's faster than this
		
		Image face = _faces;
		face.chromify(color);
		
		size_t dst_offset_x = 0;
		size_t dst_offset_y = 0;
		
		const char* cp = str;
		char c;
		while ((c = *cp++)) {
			if (c == '\n') {
				dst_offset_x = 0;
				dst_offset_y += _char_height;
				continue;
			} else if (c == '\t') {
				dst_offset_x += 8 * _char_width;
			}
			
			if (c < 0x20 || c > 0x7e)
				c = 0x7f; // unrenderable
			
			if (c != 0x20) { // don't render spaces
				byte face_index = c - 0x20;
				size_t src_x = (face_index % 0x20) * _char_width;
				size_t src_y = (face_index / 0x20) * _char_height;
				
				dst.paste(face, src_x, src_y, _char_width, _char_height, dst_x + dst_offset_x, dst_y + dst_offset_y);
			}
			
			dst_offset_x += _char_width;
		}
	}
	
	void Font::required_size_for_text(const char* str, uint32_t& w, uint32_t& h) {
		int longest_line = 0;
		int num_lines = 1;
		int line_length = 0;
		const char* cp = str;
		char c;
		while ((c = *cp++)) {
			char next = *cp;
			if (c == '\n' && next) {
				if (line_length > longest_line)
					longest_line = line_length;
				line_length = 0;
				++num_lines;
			} else if (c == '\t')
				line_length += 8;
			else
				++line_length;
		}
		if (line_length > longest_line)
			longest_line = line_length;
		
		w = longest_line * _char_width;
		h = num_lines * _char_height;
	}
}