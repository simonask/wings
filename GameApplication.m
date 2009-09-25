//
//  GameApplication.m
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GameApplication.h"


@implementation GameApplication
- (id)init {
	self = [super init];
	running = YES;
	return self;
}

- (void)run {
	NSRunLoop* loop = [[NSRunLoop currentRunLoop] retain];
	[self finishLaunching];
	
	FallingAppDelegate* delegate = (FallingAppDelegate*)[self delegate];
	EngineView* view = [delegate engineView];
	
	while (running) {
		// listen for events, send them to self (assuming that EngineView is first responder)
		while (YES) {
			NSEvent* event = [self nextEventMatchingMask:NSAnyEventMask untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
			if (event == nil)
				break;
			[self sendEvent:event];
		}
		
		[view update];
	}
	
	[loop release];
}

- (void)runOnce {
	[self run];
}
@end
