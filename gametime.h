/*
 *  time.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 11/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GAMETIME_H_
#define _GAMETIME_H_

#include "basic.h"
#include <float.h>

typedef double GameTime;

CAPI void time_start();
CAPI void time_stop();
CAPI GameTime time_tick();
CAPI GameTime time_now();
CAPI void time_set_speed(double modifier);
CAPI double time_get_speed();

static inline GameTime time_never() { return 0.0; }
static inline GameTime time_forever() { return DBL_MAX; }


#endif // _GAMETIME_H_
