/*
 *  float4.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 09/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _FLOAT4_H_
#define _FLOAT4_H_

#include "basic.h"

#include <xmmintrin.h>
#include <pmmintrin.h>
#include <inttypes.h>


// declarations
namespace wings
{

typedef __m128 float4;
struct ALIGNED(16) float4s;

union any4 {
	float4 v;
	float f[4];
	uint32_t u[4];
	int32_t n[4];
};

static const any4 wmask_xyz = { { 0xffffffff, 0xffffffff, 0xffffffff, 0x0 } };

FORCE_INLINE void wstore(float4s&, float4);
FORCE_INLINE float4 wzero();
FORCE_INLINE float4 wreplicate(float f);
FORCE_INLINE float4 wset(float x, float y, float z, float w);
FORCE_INLINE float4 wvector3(float x, float y, float z);
FORCE_INLINE float4 wvector2(float x, float y);
FORCE_INLINE float wgetx(float4 v);
FORCE_INLINE float wgety(float4 v);
FORCE_INLINE float wgetz(float4 v);
FORCE_INLINE float wgetw(float4 v);
FORCE_INLINE float4 wadd(float4 a, float4 b);
FORCE_INLINE float4 wsub(float4 a, float4 b);
FORCE_INLINE float4 wmul(float4 a, float4 b);
FORCE_INLINE float4 wand(float4 a, float4 b);
FORCE_INLINE float4 wor(float4 a, float4 b);
FORCE_INLINE float4 wxor(float4 a, float4 b);
FORCE_INLINE float4 wsum(float4 a);
FORCE_INLINE float4 wsplatx(float4 v);
FORCE_INLINE float4 wsplaty(float4 v);
FORCE_INLINE float4 wsplatz(float4 v);
FORCE_INLINE float4 wsplatw(float4 v);
FORCE_INLINE float4 wreverse(float4 v);
FORCE_INLINE float4 wror(float4 v);
FORCE_INLINE float4 wrol(float4 v);
FORCE_INLINE float4 wdot3(float4 a, float4 b);
static inline void wprint(float4 w);

struct ALIGNED(16) float4s {
	union {
		float4 m;
		struct { float x, y, z, w; };
		float v[4];
	};
};

// definitions
	
void wstore(float4s& v, float4 m) {
	_mm_store_ps((float*)&v.v, m);
}

float4 wzero() {
	return _mm_setzero_ps();
}

float4 wreplicate(float f) {
	return _mm_set1_ps(f);
}

float4 wset(float x, float y, float z, float w) {
	// reversed component order, because SSE is weird (_mm_set_ps and _mm_load_ps load in opposite order)
	return _mm_set_ps(w, z, y, x);
}

float4 wvector3(float x, float y, float z) {
	return wset(x, y, z, 1.f);
}

float4 wvector2(float x, float y) {
	return wvector3(x, y, 0.f);
}

float wgetx(float4 v) {
	float4s s;
	wstore(s, v);
	return s.x;
}

float wgety(float4 v) {
	float4s s;
	wstore(s, v);
	return s.y;
}

float wgetz(float4 v) {
	float4s s;
	wstore(s, v);
	return s.z;
}

float wgetw(float4 v) {
	float4s s;
	wstore(s, v);
	return s.w;
}

float4 wadd(float4 a, float4 b) {
	return a + b;
}

float4 wsub(float4 a, float4 b) {
	return a - b;
}

float4 wmul(float4 a, float4 b) {
	return a * b;
}

float4 wand(float4 a, float4 b) {
	return a & b;
}

float4 wor(float4 a, float4 b) {
	return a | b;
}

float4 wxor(float4 a, float4 b) {
	return a ^ b;
}

float4 wsum(float4 a) {
	// (x, y, z, w) -> (x+y+z+w, ..., ..., ...)
	
	// TODO: SSE2 version
	a = _mm_hadd_ps(a, a);
	return _mm_hadd_ps(a, a);
}

float4 wsplatx(float4 v) {
	// (x, y, z, w) -> (x, x, x, x)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0));
}

float4 wsplaty(float4 v) {
	// (x, y, z, w) -> (y, y, y, y)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1));
}

float4 wsplatz(float4 v) {
	// (x, y, z, w) -> (z, z, z, z)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2));
}

float4 wsplatw(float4 v) {
	// (x, y, z, w) -> (w, w, w, w)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3));
}

float4 wreverse(float4 v) {
	// (x, y, z, w) -> (w, z, y, x)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 1, 2, 3));
}

float4 wrol(float4 v) {
	// (x, y, z, w) -> (w, x, y, z)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 3, 2, 1));
}

float4 wror(float4 v) {
	// (x, y, z, w) -> (y, z, w, x)
	return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 1, 0, 3));
}

float4 wdot3(float4 a, float4 b) {
#ifdef __SSE3__
	register float4 m = wand(wmul(a, b), wmask_xyz.v);
	return wsum(m);
#else
	register float4 m = wmul(a, b);
	register float4 r = m;
	m = wror(m);
	r = _mm_add_ss(r, m);
	m = wror(m);
	r = _mm_add_ss(r, m);
	return wsplatx(r);
#endif
}

void wprint(float4 w)
{
	float4s v;
	wstore(v, w);
	printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

}

#endif // _FLOAT4_H_

