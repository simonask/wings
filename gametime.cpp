/*
 *  time.c
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 11/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "gametime.h"
#include <sys/time.h>

namespace wings {
namespace time {
	
	static double last_system_tick = 0.0;
	static GameTime _now = 0.0;
	static double speed_modifier = 0.0;
	static bool started = false;
	
	SystemTime system_now() {
		struct timeval t;
		gettimeofday(&t, NULL);
		return (double)t.tv_sec + (double)t.tv_usec / 1000000.0;
	}
	
	void start() {
		last_system_tick = system_now();
		started = true;
	}
	
	void stop() {
		started = false;
	}
	
	GameTime tick() {
		if (started) {
			double next_system_tick = system_now();
			double system_delta = next_system_tick - last_system_tick;
			last_system_tick = next_system_tick;
			if (speed_modifier > 0.0) {
				double delta = system_delta / speed_modifier;
				_now += delta;
				return delta;
			}
		}
		return 0.0;
	}
	
	GameTime now() {
		return _now;
	}
	
	void set_speed(double s) {
		ASSERT(s >= 0.0);
		speed_modifier = s;
	}
	GameTime get_speed() { return speed_modifier; }
}
}