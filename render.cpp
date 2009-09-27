/*
 *  render.cpp
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 25/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "render.h"
#include "array.h"
#include "font.h"
#include "game.h"

namespace wings {
	
	
	
namespace debug {
	
	Font* debug_font = NULL;
	
	static Array<DebugRender*> debug_render_objects;
	
	void _register_debug_render_object(DebugRender* r) {
		debug_render_objects.push(r);
	}
	
	void render() {
		SystemTime now = time::system_now();
		
		for (size_t i = 0; i < debug_render_objects.size();) {
			DebugRender* r = debug_render_objects[i];
			r->render();
			if (r->expiry_time() < now) {
				delete debug_render_objects[i];
				debug_render_objects.erase(i);
			} else {
				++i;
			}
		}
	}
	
	void DebugText::render() {
		if (!debug_font) {
			Image font_face;
			Image::load_png(font_face, game::resource_path("pixelfont.png"));
			debug_font = new Font(font_face, 6, 8);
		}
		
		if (!_texture_generated) {
			glGenTextures(1, &_texture_id);
			glBindTexture(GL_TEXTURE_2D, _texture_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			std::string str = _ss.str();
			const char* cstr = str.c_str();
			
			debug_font->required_size_for_text(cstr, _w, _h);
			Image rendered_text(_w, _h);
			debug_font->render(rendered_text, 0, 0, cstr, COLOR_YELLOW);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const byte*>(rendered_text.pixels()));

			_texture_generated = true;
		}
		else {
			glBindTexture(GL_TEXTURE_2D, _texture_id);
		}

		
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(_col * debug_font->char_width(), _row * debug_font->char_height(), 0.0);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0, 0.0);
			glVertex2i(0, 0);
			glTexCoord2d(1.0, 0.0);
			glVertex2i(_w, 0);
			glTexCoord2d(1.0, 1.0);
			glVertex2i(_w, _h);
			glTexCoord2d(0.0, 1.0);
			glVertex2i(0, _h);
		}
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
	
	DebugText::~DebugText() {
		if (_texture_generated) {
			glDeleteTextures(1, &_texture_id);
		}
	}
}
	
}
