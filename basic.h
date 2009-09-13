/*
 *  basic.h
 *  falling
 *
 *  Created by Simon Ask Ulsnes on 09/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define FORCE_INLINE static inline __attribute__((always_inline))
#define ALIGNED(x) __attribute__((aligned(x)))
#define TRAP() {__asm__("int3\nnop\n");}
#define ASSERT(x) if (!(x)){TRAP();}

#ifdef __cplusplus
#define CAPI extern "C"
#else
#define CAPI
#endif