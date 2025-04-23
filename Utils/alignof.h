#ifndef ALIGNOF_H
#define ALIGNOF_H

#include <stddef.h>
#include "macros.h"

#if __STDC_VERSION__ < 201112L
	#define BUILD_ALIGNMENT_STRUCT(type) \
	struct CAT(type, _ALIGNMENT) {\
		char a;\
		type b;\
	};
#ifndef _Alignof
	#define _Alignof(type) offsetof(struct CAT(type, _ALIGNMENT), b)
#endif
#else
	#define BUILD_ALIGNMENT_STRUCT(type)
#endif

#endif
