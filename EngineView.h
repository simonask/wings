//
//  EngineView.h
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface EngineView : NSView {
	NSOpenGLContext* gl;
}
-(void)engineResize;
-(void)update;

@end
