//
//  EngineView.h
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "engine.h"


@interface EngineView : NSView {
	Engine* engine;
	NSOpenGLContext* gl;
}
@property (assign) Engine* engine;
-(void)setupEngine:(Engine*)engine;
-(void)update;

@end
