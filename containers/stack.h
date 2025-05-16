#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "mutsequence.h"

#define Stack(type) \
struct DELIM(Stack, _, type);

#define DECLARE_Stack(type) \
typedef int CAT(Stack(type), Append_f)(INTERFACE_PARAM(Stack(type)), type); \
typedef type CAT(Stack(type), Pop_f)(INTERFACE_PARAM(Stack(type))); \
DECLARE_INTERFACE_START(Stack(type)) \
    CAT(Stack(type), Append_f) * append; \
    CAT(Stack(type), Pop_f) * pop; \
    MutSequence(type) * mutsequence; \
    Sequence(type) * sequence; \
    Container(type) * container; \
    Iterable(type) * iterable; \
    Iterator(type) * iterator; \
DECLARE_INTERFACE_END

#endif
