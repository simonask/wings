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

static double last_system_tick = 0.0;
static GameTime now = 0.0;
static double speed_modifier = 0.0;
static bool started = false;

static double get_current_system_time() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec / 1000000.0;
}

void time_start() {
	last_system_tick = get_current_system_time();
	started = true;
}

void time_stop() {
	started = false;
}

GameTime time_tick() {
	if (started) {
		double next_system_tick = get_current_system_time();
		double system_delta = next_system_tick - last_system_tick;
		last_system_tick = next_system_tick;
		if (speed_modifier > 0.0) {
			double delta = system_delta / speed_modifier;
			now += delta;
			return delta;
		}
	}
	return 0.0;
}

GameTime time_now() {
	return now;
}

void time_set_speed(double s) {
	ASSERT(s >= 0.0);
	speed_modifier = s;
}
GameTime time_get_speed() { return speed_modifier; }
