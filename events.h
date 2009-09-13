/*
 *  events.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 12/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EVENT_H_
#define _EVENT_H_

typedef enum {
	EVENT_TYPE_KEY_DOWN     = 1,
	EVENT_TYPE_KEY_UP       = 1 << 1,
	EVENT_TYPE_MOUSE_DOWN   = 1 << 10,
	EVENT_TYPE_MOUSE_UP     = 1 << 11,
	EVENT_TYPE_MOUSE_MOVED  = 1 << 12,
	EVENT_TYPE_MOUSE_SCROLL = 1 << 13,
	
	EVENT_TYPE_ALL          = 0xffffffff
} EventType;

typedef enum {
	MOUSE_BUTTON_NONE,
	MOUSE_BUTTON_FIRST,
	MOUSE_BUTTON_SECOND,
	MOUSE_BUTTON_THIRD,
	MOUSE_BUTTON_FOURTH,
	MOUSE_BUTTON_SCROLL,
} MouseButton;

typedef struct {
	EventType type;
	union {
		struct {
			int key;
			unsigned int modifiers;
			const char* str;
		} key;
		
		struct {
			MouseButton button;
			double x;
			double y;
		} mouse;
	};
} Event;

typedef void(*EventListenerFunc)(const Event* event, void* data);

#endif // _EVENT_H_