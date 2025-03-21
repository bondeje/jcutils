#ifndef ALIGNOF_H
#define ALIGNOF_H

#include <stddef.h>

#if __STDC_VERSION__ < 201112L
#ifndef _Alignof
	#define _Alignof(type) offsetof(struct CAT(type, _ALIGNMENT), b)
#endif

#ifndef CAT
  	#define CAT_(x, y) x##y
  	#define CAT(x, y) CAT_(x, y)
#endif
	#define BUILD_ALIGNMENT_STRUCT(type) \
	struct CAT(type, _ALIGNMENT) {\
		char a;\
		type b;\
	};
#else
	#define BUILD_ALIGNMENT_STRUCT(type)
#endif

#endif
