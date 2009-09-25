/*
 *  placement_new.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 24/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef WINGS_PLACEMENT_NEW_H
#define WINGS_PLACEMENT_NEW_H

void* operator new(size_t sz, void* ptr) {
	return ptr;
}

#endif
