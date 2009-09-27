//
//  EngineView.m
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "EngineView.h"
#include "events.h"
#include "game.h"
using namespace wings;

@implementation EngineView

- (id)initWithFrame:(NSRect)frame {
	ASSERT(!game::is_initialized()); // only one EngineView per process, please!
	NSString* resource_path = [[NSBundle mainBundle] resourcePath];
	game::initialize([resource_path UTF8String]);
	
	self = [super initWithFrame:frame];
	if (self) {
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
		game::setup_opengl();
		
		[self engineResize];
	}
	return self;
}

- (void)dealloc {
	game::deinitialize();
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
	game::window_resized((float)frame.size.width, (float)frame.size.height);
}

- (void)update {
	game::update();
	
	if ([gl view] != self)
		[gl setView:self];
	
	if (game::needs_render())
		[self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
	[gl makeCurrentContext];
	game::render();
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
	game::send_event(e);
}

- (void)keyUp:(NSEvent *)theEvent {
	Event e;
	e.type = EVENT_TYPE_KEY_UP;
	e.key.key = [theEvent keyCode];
	e.key.modifiers = (unsigned int)([theEvent modifierFlags] >> 16);
	game::send_event(e);
}

- (void)engineMouseEvent:(EventType)type button:(MouseButton)button {
	Event e;
	e.type = type;
	e.mouse.button = button;
	
	NSPoint point = [self convertPointFromBase:[[self window] mouseLocationOutsideOfEventStream]];
	e.mouse.x = point.x;
	e.mouse.y = point.y;
	
	game::send_event(e);
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
	game::send_event(e);
}

- (void)mouseMoved:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_MOVED button:MOUSE_BUTTON_NONE];
}

- (void)mouseDragged:(NSEvent *)theEvent {
	[self engineMouseEvent:EVENT_TYPE_MOUSE_MOVED button:MOUSE_BUTTON_FIRST];
}

@end
