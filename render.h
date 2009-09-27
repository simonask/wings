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
#include "gametime.h"

#include <sstream>
#include <OpenGL/gl.h>

namespace wings
{
	typedef void(*RenderFunc)(void* userdata);
	typedef void(*RenderGetPositionFunc)(void* userdata, float4s& out); // 3D position (x, y, z, _), used for depth-sorting
	typedef void(*RenderGetBoundingBoxFunc)(void* userdata, float4s& out); // 2D bounding box (x, y, w, h)
	
	void render_text(const char* text, float x, float y, Color color);
	
	namespace debug
	{
		void render();
		
		class DebugRender {
		private:
			GameTime _expire;
		public:
			DebugRender(GameTime ttl = -1.0) {
				_expire = ttl > 0.0 ? time::now() + ttl : (ttl == 0.0 ? 0.0 : time::FOREVER);
			}
			GameTime expiry_time() const { return _expire; }
			virtual void render() = 0;
		};
		
		void _register_debug_render_object(DebugRender* r);
		template <typename T>
		T& register_debug_render_object(T* obj) {
			_register_debug_render_object(obj);
			return *obj;
		}
		
		class DebugText : public DebugRender {
			template <typename T> friend DebugText& operator<<(DebugText&, const T&);
		private:
			uint32_t _col, _row;
			uint32_t _w, _h;
			std::stringstream _ss; // TODO: Something more efficient?
			GLuint _texture_id;
			bool _texture_generated;
		public:
			DebugText(uint32_t col, uint32_t row, GameTime ttl = -1.0) : DebugRender(ttl), _col(col), _row(row), _texture_id(0), _texture_generated(false) {}
			~DebugText();
			void render();
		};
		template <typename T>
		DebugText& operator<<(DebugText& dt, const T& val) {
			dt._ss << val;
			return dt;
		}
		
		inline DebugText& text(uint32_t col, uint32_t row, GameTime ttl = -1.0) {
			return register_debug_render_object(new DebugText(col, row, ttl));
		}
	}
}

#endif // _RENDER_H_
