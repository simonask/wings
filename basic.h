/*
 *  basic.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 09/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef WINGS_BASIC_H
#define WINGS_BASIC_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define FORCE_INLINE inline __attribute__((always_inline))
#define ALIGNED(x) __attribute__((aligned(x)))
#define VECTOR_TYPE(size) __attribute__((__vector_size__(size)))
#define TRAP() {__asm__("int3\nnop\n");}
#define ASSERT(x) if (!(x)){TRAP();}

#ifdef __cplusplus__
#define CAPI extern "C"
#else
#define CAPI
#endif

typedef unsigned char byte;

#endif
