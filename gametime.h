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

namespace wings {
	typedef double GameTime;   // in-game seconds since engine init
	typedef double SystemTime; // real-life seconds since UNIX epoch
	
	namespace time {
		void start();
		void stop();
		GameTime tick();
		GameTime now();
		void set_speed(double modifier);
		double get_speed();
		SystemTime system_now();
		
		static const GameTime NEVER = 0.0;
		static const GameTime FOREVER = DBL_MAX;
	}
}


#endif // _GAMETIME_H_
