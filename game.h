/*
 *  game.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 24/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "basic.h"
#include "events.h"
#include "str.h"

namespace wings {
	
namespace game {
	void initialize(const char* resource_base_path);
	void deinitialize();
	bool is_initialized();
	void start();
	void pause();
	void stop();
	bool is_running();
	
	void setup_opengl();
	void update();
	bool needs_render();
	void render();
	
	void send_event(const Event& event);
	void register_event_listener(uint32_t type_mask, EventListenerFunc callback, void* userdata);
	void unregister_event_listener(EventListenerFunc callback, void* userdata);
	
	void window_resized(float width, float height);
	
	String resource_path(const String& subpath);
}
					
}

