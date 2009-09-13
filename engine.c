/*
 *  engine.c
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 10/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "engine.h"
#include <OpenGL/gl.h>
#include <stdlib.h>

static const GameTime GAME_UPDATE_INTERVAL = 1.0 / 60.0; // 60 game updates per game second

struct EventCallback {
	unsigned int type_mask;
	EventListenerFunc callback;
	void* userdata;
};

struct Renderable {
	void* userdata;
	RenderFunc render;
	RenderGetPositionFunc get_position;
	RenderGetBoundingBoxFunc get_bounding_box;
	
	// cache some things
	float4s position;
	float4s bounds;
};

struct EngineInternal {
	struct EventCallback* ec;
	size_t ec_offset;
	size_t ec_size;
	
	struct Renderable* rc;
	size_t rc_offset;
	size_t rc_size;
	
	float zoom_factor;
	float4s camera_position;
	float4s world_bounds;
	float4s window_bounds;
};

Engine* engine_initialize() {
	Engine* e = (Engine*)malloc(sizeof(Engine));
	e->running = false;
	e->paused = false;
	e->last_game_update = time_never();
	e->last_frame_update = time_never();
	e->needs_render = false;
	
	e->internal = (struct EngineInternal*)malloc(sizeof(struct EngineInternal));
	
	e->internal->ec = malloc(1024 * sizeof(struct EventCallback));
	e->internal->ec_offset = 0;
	e->internal->ec_size = 1024;
	
	e->internal->rc = malloc(1024 * sizeof(struct Renderable));
	e->internal->rc_offset = 0;
	e->internal->rc_size = 1024;
	
	return e;
}

void engine_setup_opengl(Engine* e) {
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void engine_destroy(Engine* e) {
	ASSERT(!e->running); // please don't destroy a running engine.
	free(e);
}

void engine_start(Engine* e) {
	e->running = true;
	e->paused = false;
	time_start();
}

void engine_pause(Engine* e) {
	e->paused = true;
	time_stop();
}

void engine_stop(Engine* e) {
	e->running = false;
	e->paused = false;
	time_stop();
}

static void game_update(Engine* e, GameTime delta) {
	
}

void engine_game_update(Engine* e) {
	time_tick();
	GameTime now = time_now();
	double last_update = e->last_game_update;
	for (; last_update < now; last_update += GAME_UPDATE_INTERVAL) {
		game_update(e, GAME_UPDATE_INTERVAL);
	}
	e->needs_render = true;
}

void engine_resize_window(Engine* e, float width, float height) {
	e->internal->window_bounds.x = width;
	e->internal->window_bounds.y = height;
	glViewport(0, 0, width, height);
	e->needs_render = true;
}

static int compare_by_z_depth(const void* _a, const void* _b) {
	const struct Renderable* a = (const struct Renderable*)_a;
	const struct Renderable* b = (const struct Renderable*)_b;
	float d = b->position.z - a->position.z;
	
	if (d < 0.0f)
		return -1;
	else if (d > 0.0f)
		return 1;
	else
		return 0;
}

static double zoom_factor = 1.0;

void engine_render_pass(Engine* e) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// camera lens
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float half_width = e->internal->window_bounds.x / 2.0f;
	float half_height = e->internal->window_bounds.y / 2.0f;
	glFrustum(zoom_factor * -half_width, zoom_factor * half_width, zoom_factor * half_height, zoom_factor * -half_height, 1.0, 100.0);
	
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
	
	
	// objects
	for (size_t i = 0; i < e->internal->rc_offset; ++i) {
		struct Renderable* r = e->internal->rc + i;
		r->get_position(r->userdata, &r->position);
		r->get_bounding_box(r->userdata, &r->bounds);
	}
	
	// TODO: bounds checking before sorting?
	qsort(e->internal->rc, e->internal->rc_offset, sizeof(struct Renderable), compare_by_z_depth);
	
	for (size_t i = 0; i < e->internal->rc_offset; ++i) {
		struct Renderable* r = e->internal->rc + i;
		
		glPushMatrix();
		glTranslatef(r->position.x, r->position.y, r->position.z);
		r->render(r->userdata);
		glPopMatrix();
	}
	
	// 2D overlay / HUD
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, half_width*2, half_height*2, 0.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_QUADS);
	{
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(100.f, 0.0f);
		glVertex2f(100.f, 100.f);
		glVertex2f(0.0f, 100.f);
	}
	glEnd();
}

void engine_register_for_render(Engine* engine, RenderFunc render, RenderGetPositionFunc get_position, RenderGetBoundingBoxFunc get_bounding_box, void* userdata) {
	ASSERT(engine->internal->rc_offset <= engine->internal->rc_size);
	
	// resize list of callbacks, if necessary
	if (engine->internal->rc_offset == engine->internal->rc_size) {
		size_t new_size = engine->internal->rc_size * 2;
		engine->internal->rc = realloc(engine->internal->rc, new_size);
		engine->internal->rc_size = new_size;
	}
	
	size_t i = engine->internal->rc_offset++;
	struct Renderable* rc = engine->internal->rc + i;
	rc->userdata = userdata;
	rc->render = render;
	rc->get_position = get_position;
	rc->get_bounding_box = get_bounding_box;
}


void engine_send_event(Engine* engine, const Event* event) {
	for (size_t i = 0; i < engine->internal->ec_offset; ++i) {
		struct EventCallback* ec = engine->internal->ec + i;
		if (ec->type_mask & event->type) {
			ec->callback(event, ec->userdata);
		}
	}
}

void engine_register_event_listener(Engine* engine, unsigned int event_type_mask, EventListenerFunc callback, void* userdata) {
	ASSERT(engine->internal->ec_offset <= engine->internal->ec_size);
	
	// resize list of callbacks, if necessary
	if (engine->internal->ec_offset == engine->internal->ec_size) {
		size_t new_size = engine->internal->ec_size * 2;
		engine->internal->ec = realloc(engine->internal->ec, new_size);
		engine->internal->ec_size = new_size;
	}
	
	size_t i = engine->internal->ec_offset++;
	struct EventCallback* ec = engine->internal->ec + i;
	ec->type_mask = event_type_mask;
	ec->callback = callback;
	ec->userdata = userdata;
}

void engine_unregister_event_listener(Engine* engine, void* userdata) {
	ASSERT(false); // TODO
}


