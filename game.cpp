/*
 *  game.cpp
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 24/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "game.h"
#include "gametime.h"
#include "float4.h"
#include "array.h"
#include "render.h"
#include "str.h"
#include <OpenGL/gl.h>

#include <iomanip>

namespace wings {
namespace game {
	static bool initialized = false;
	static bool running = false;
	static bool paused = false;
	static GameTime last_game_update = time::NEVER;
	static SystemTime last_render = time::NEVER;
	static double fps = 0.0;
	static String resource_base_path;
		
	static struct {
		float zoom;
		float4 position;
	} camera;
		
	static float4s world_size;
	static float4s window_size;
	
	struct EventCallback {
		unsigned int type_mask;
		EventListenerFunc callback;
		void* userdata;
	};
	static Array<EventCallback> event_callbacks;
	
	struct RenderCallback {
		void* userdata;
		RenderFunc render;
		RenderGetPositionFunc get_position;
		RenderGetBoundingBoxFunc get_bounding_box;
		
		float4s position;
		float4s bounding_box;
		
		bool operator<(const RenderCallback& other) const {
			return position.z < other.position.z;
		}
		bool operator==(const RenderCallback& other) const {
			return position.z == other.position.z;
		}
	};
	static Array<RenderCallback> render_callbacks;
	
	
	void initialize(const char* base_path) {
		resource_base_path = base_path;
		resource_base_path += "/";
		camera.zoom = 1.0f;
		initialized = true;
		event_callbacks.preallocate(256);
	}
	
	void deinitialize() {
		// TODO: Release all game data
		initialized = false;
	}
	
	bool is_initialized() {
		return initialized;
	}
	
	void start() {
		running = true;
		paused = false;
	}
	
	void stop() {
		running = false;
		paused = false;
	}
	
	void pause() {
		paused = true;
	}
	
	bool is_running() { return running; }
	
	void setup_opengl() {
		glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	
	void update() {
		ASSERT(initialized);
		static bool ddd = false;
		if (!ddd) {
			debug::text(10, 10) << "Hello World!";
			ddd = true;
		}
		
		last_game_update = time::now();
	}
	
	bool needs_render() {
		return true;
	}
	
	void render() {
		ASSERT(initialized);
		
		float half_width;
		float half_height;
		
		#pragma omp parallel sections
		{
			// clear screen and sort renderables at the same time
			#pragma omp section
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
				// camera lens
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				half_width = window_size.x / 2.0f;
				half_height = window_size.y / 2.0f;
				glFrustum(camera.zoom * -half_width, camera.zoom * half_width, camera.zoom * half_height, camera.zoom * -half_height, 1.0, 100.0);
				
				// camera position
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0.0f, 0.0f, -1.f);
				
				// background
				glBegin(GL_QUADS);
				{
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f(-half_width, -half_height);
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex2f(half_width, -half_height);
					glColor3f(0.0f, 0.0f, 1.0f);
					glVertex2f(half_width, half_height);
					glColor3f(1.0f, 1.0f, 0.0f);
					glVertex2f(-half_width, half_height);
				}
				glEnd();
			}
		
			#pragma omp section
			{
				#pragma omp parallel for
				for (size_t i = 0; i < render_callbacks.size(); ++i) {
					RenderCallback& c = render_callbacks[i];
					c.get_position(c.userdata, c.position);
				}
				
				render_callbacks.qsort();
			}
		}
		
		// render!
		for (size_t i = 0; i < render_callbacks.size(); ++i) {
			render_callbacks[i].render(render_callbacks[i].userdata);
		}
		
		// 2D overlay / HUD
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, half_width*2, half_height*2, 0.0, -1.0, 1.0);
		
		SystemTime now = time::system_now();
		SystemTime diff = now - last_render;
		fps = 1.0 / diff;
		
		debug::text(0, 0, 0) << std::setprecision(4) << std::setw(5) << std::setfill('0') << fps << " fps";
		
		debug::render();
		
		last_render = now;
	}
	
	void send_event(const Event& e) {
		for (size_t i = 0; i < event_callbacks.size(); ++i) {
			EventCallback& c = event_callbacks[i];
			if (c.type_mask & e.type)
			{
				c.callback(e, c.userdata);
			}
		}
	}
	
	void register_event_listener(uint32_t type_mask, EventListenerFunc callback, void* userdata) {
		EventCallback& c = event_callbacks.push();
		c.type_mask = type_mask;
		c.callback = callback;
		c.userdata = userdata;
	}
	
	void unregister_event_listener(EventListenerFunc callback, void* userdata) {
		TRAP(); // XXX: TODO
	}
	
	void window_resized(float width, float height) {
		window_size.x = width;
		window_size.y = height;
		glViewport(0, 0, width, height);
	}
	
	String resource_path(const String& subpath) {
		return resource_base_path + subpath;
	}
}
}
