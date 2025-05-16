#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "container.h"
#include "iterable.h"

#define Sequence(type) \
DELIM(Sequence, _, type)

#define DECLARE_Sequence(type) \
struct Sequence(type); \
struct VTABLE_TYPE(Sequence(type)); \
typedef type * CAT(Sequence(type), _Get_f)(INTERFACE_PARAM(Sequence(type)), intptr_t); \
typedef int CAT(Sequence(type), _Set_f)(INTERFACE_PARAM(Sequence(type)), intptr_t, type); \
typedef void CAT(Sequence(type), _Reverse_f)(INTERFACE_PARAM(Sequence(type))); \
DECLARE_INTERFACE_START(Sequence(type)) \
    CAT(Sequence(type), _Get_f) * get; \
    CAT(Sequence(type), _Set_f) * set; \
    CAT(Sequence(type), _Reverse_f) * reverse; \
    intptr_t Container(type); \
    intptr_t Iterable(type); \
DECLARE_INTERFACE_END

#endif
