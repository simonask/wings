//
//  EngineView.m
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "EngineView.h"
#include "events.h"

@implementation EngineView
@synthesize engine;

- (id)initWithFrame:(NSRect)frame {
	self = [super initWithFrame:frame];
	if (self) {
		gl = nil;
	}
	return self;
}

- (void)dealloc {
	if (gl != nil)
		[gl release];
	[super dealloc];
}

- (BOOL)isOpaque {
	return YES;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (BOOL)becomeFirstResponder {
	return YES;
}

- (void)engineResize {
	NSRect frame = [self bounds];
	[gl update];
	[gl makeCurrentContext];
	engine_resize_window(engine, (float)frame.size.width, (float)frame.size.height);
}

- (void)setupEngine:(Engine*)e {
	self.engine = e;
	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 0,
		0
	};
	
	id attr = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	gl = [[NSOpenGLContext alloc] initWithFormat:attr shareContext:nil];
	[attr release];
	
	[gl makeCurrentContext];
	engine_setup_opengl(engine);
	
	[self engineResize];
}

- (void)update {
	engine_game_update(engine);
	
	if ([gl view] != self)
		[gl setView:self];
	
	if (engine_needs_render(engine))
		[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
	if (!engine) return;
	
	[gl makeCurrentContext];
	engine_render_pass(engine);
	[gl flushBuffer];
}

- (void)resizeWithOldSuperviewSize:(NSSize)oldSize {
	[super resizeWithOldSuperviewSize:oldSize];
	[self engineResize];
}

- (void)keyDown:(NSEvent *)theEvent {
	Event e;
	e.type = EVENT_TYPE_KEY_DOWN;
	e.key.key = [theEvent keyCode];
	e.key.modifiers = (unsigned int)([theEvent modifierFlags] >> 16);
	engine_send_event(engine, &e);
}

- (void)keyUp:(NSEvent *)theEvent {
	Event e;
	e.type = EVENT_TYPE_KEY_UP;
	e.key.key = [theEvent keyCode];
	e.key.modifiers = (unsigned int)([theEvent modifierFlags] >> 16);
	engine_send_event(engine, &e);
}

- (void)engineMouseEvent:(EventType)type button:(MouseButton)button {
	Event e;
	e.type = type;
	e.mouse.button = button;
	
	NSPoint point = [self convertPointFromBase:[[self window] mouseLocationOutsideOfEventStream]];
	e.mouse.x = point.x;
	e.mouse.y = point.y;
	
	engine_send_event(engine, &e);
}

- (void)mouseDown:(NSEvent*)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_DOWN button:MOUSE_BUTTON_FIRST];
}

- (void)mouseUp:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_UP button:MOUSE_BUTTON_FIRST];
}

- (void)rightMouseDown:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_DOWN button:MOUSE_BUTTON_SECOND];
}

- (void)rightMouseUp:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_UP button:MOUSE_BUTTON_SECOND];
}

- (void)otherMouseDown:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_DOWN button:MOUSE_BUTTON_THIRD]; // todo
}

- (void)otherMouseUp:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_UP button:MOUSE_BUTTON_THIRD]; // todo
}

- (void)scrollWheel:(NSEvent *)theEvent {
	Event e;
	e.type = EVENT_TYPE_MOUSE_SCROLL;
	e.mouse.button = MOUSE_BUTTON_SCROLL;
	e.mouse.x = [theEvent deltaX];
	e.mouse.y = [theEvent deltaY];
	engine_send_event(engine, &e);
}

- (void)mouseMoved:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_MOVED button:MOUSE_BUTTON_NONE];
}

- (void)mouseDragged:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_MOVED button:MOUSE_BUTTON_FIRST];
}

@end
