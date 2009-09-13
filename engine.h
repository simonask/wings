/*
 *  engine.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 10/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "basic.h"
#include "gametime.h"
#include "events.h"
#include "render.h"

struct EngineInternal;

typedef struct Engine {
	bool running;
	bool paused;
	bool needs_render;
	GameTime last_game_update;
	GameTime last_frame_update;
	struct EngineInternal* internal;
} Engine;

CAPI Engine* engine_initialize();
CAPI void engine_setup_opengl(Engine*);
CAPI void engine_destroy(Engine*);
CAPI void engine_start(Engine*);
CAPI void engine_stop(Engine*);
CAPI void engine_pause(Engine*);
static inline bool engine_is_running(Engine* engine) { return engine->running; }
CAPI void engine_game_update(Engine*);
CAPI void engine_resize_window(Engine*, float width, float height);

CAPI void engine_send_event(Engine*, const Event*);
CAPI void engine_register_event_listener(Engine*, unsigned int event_type_mask, EventListenerFunc callback, void* userdata);
CAPI void engine_unregister_event_listener(Engine*, void* userdata);

static inline bool engine_needs_render(Engine* e) { return e->needs_render; }
CAPI void engine_render_pass(Engine*);
CAPI void engine_register_for_render(Engine*, RenderFunc render_func, RenderGetPositionFunc get_position_func, RenderGetBoundingBoxFunc get_bounding_box_func, void* userdata);
CAPI void engine_unregister_for_render(Engine*, void* userdata);

#endif // _ENGINE_H_
