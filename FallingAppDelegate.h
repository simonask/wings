//
//  fallingAppDelegate.h
//  falling
//
//  Created by Simon Ask Ulsnes on 09/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EngineView.h"

@interface FallingAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	IBOutlet EngineView* engineView;
	NSOpenGLContext* gl;
}

@property (assign) EngineView* engineView;
@property (assign) IBOutlet NSWindow *window;

@end

