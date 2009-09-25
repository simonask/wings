//
//  GameApplication.h
//  falling
//
//  Created by Simon Ask Ulsnes on 10/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "FallingAppDelegate.h"
//#include "engine.h"


@interface GameApplication : NSApplication {
	BOOL running;
}
-(void)run;
-(void)runOnce;

@end
