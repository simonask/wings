//
//  fallingAppDelegate.m
//  falling
//
//  Created by Simon Ask Ulsnes on 09/09/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "FallingAppDelegate.h"

@implementation FallingAppDelegate

@synthesize window;
@synthesize engineView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	[window setAcceptsMouseMovedEvents:YES];
}

-(void)awakeFromNib {
	NSLog(@"VÅGNEDE OP!");
}

@end
