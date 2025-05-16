#ifndef MUTSEQUENCE_H
#define MUTSEQUENCE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sequence.h"

#define MutSequence(type) \
struct DELIM(MutSequence, _, type);

#define DECLARE_MutSequence(type) \
typedef int CAT(MutSequence(type), Delete_f)(INTERFACE_PARAM(MutSequence(type)), intptr_t); \
typedef int CAT(MutSequence(type), Insert_f)(INTERFACE_PARAM(MutSequence(type)), intptr_t, type); \
typedef void CAT(MutSequence(type), Clear_f)(INTERFACE_PARAM(MutSequence(type))); \
typedef int CAT(MutSequence(type), Extend_f)(INTERFACE_PARAM(MutSequence(type)), INTERFACE_PARAM(Iterable(type))); \
DECLARE_INTERFACE_START(MutSequence(type)) \
    CAT(MutSequence(type), Delete_f) * delete; \
    CAT(MutSequence(type), Insert_f) * insert; \
    CAT(MutSequence(type), Clear_f) * clear; \
    CAT(MutSequence(type), Extend_f) * extend; \
    Sequence(type) * sequence; \
    Container(type) * container; \
    Iterable(type) * iterable; \
    Iterator(type) * iterator; \
DECLARE_INTERFACE_END

#endif
