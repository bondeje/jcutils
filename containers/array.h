#ifndef ARRAY_H
#define ARRAY_H

#include <string.h>
#include "sequence.h"

#define Array(type) \
DELIM(Array, _, type)

#define IteratorArray(type) \
DELIM(IteratorArray, _, type)

#define Array_initer(type_) \
. Container(type_) = INTERFACE_INITIALIZER(Array(type_), Container),\
. Iterable(type_) = INTERFACE_INITIALIZER(Array(type_), Iterable),\
. Sequence(type_) = INTERFACE_INITIALIZER(Array(type_), Sequence),\

#define DECLARE_Array(type) \
struct IteratorArray(type) { \
    type * array; \
    intptr_t start, cur, stop, step; \
    IMPLEMENTS(Iterable(type)); \
    IMPLEMENTS(Iterator(type)); \
}; \
extern struct VTABLE_TYPE(Iterable(type)) MANGLE2(IteratorArray(type), Iterable_inter); \
extern struct VTABLE_TYPE(Iterator(type)) MANGLE2(IteratorArray(type), Iterator_inter); \
type * CAT(IteratorArray(type), _Iterator_next)(INTERFACE_PARAM(Iterator(type))); \
void CAT(IteratorArray(type), _Iterable_iter)(INTERFACE_PARAM(Iterable(type)), struct Iterable(type) *); \
struct Array(type) { \
    type * array; \
    intptr_t size; \
    IMPLEMENTS(Container(type)); \
    IMPLEMENTS(Iterable(type)); \
    IMPLEMENTS(Sequence(type)); \
    bool reversed; \
}; \
extern struct VTABLE_TYPE(Container(type)) MANGLE2(Array(type), Container_inter); \
extern struct VTABLE_TYPE(Iterable(type)) MANGLE2(Array(type), Iterable_inter); \
extern struct VTABLE_TYPE(Sequence(type)) MANGLE2(Array(type), Sequence_inter); \
bool CAT(Array(type), _Container_contains)(INTERFACE_PARAM(Container(type)), const type, int (*)(const type *, const type *)); \
bool CAT(Array(type), _Container_is_empty)(INTERFACE_PARAM(Container(type))); \
intptr_t CAT(Array(type), _Container_len)(INTERFACE_PARAM(Container(type))); \
void CAT(Array(type), _Iterable_iter)(INTERFACE_PARAM(Iterable(type)), struct Iterable(type) *); \
type * CAT(Array(type), _Sequence_get)(INTERFACE_PARAM(Sequence(type)), intptr_t); \
int CAT(Array(type), _Sequence_set)(INTERFACE_PARAM(Sequence(type)), intptr_t, type); \
void CAT(Array(type), _Sequence_reverse)(INTERFACE_PARAM(Sequence(type))); \

#define DEFINE_Array(type) \
struct VTABLE_TYPE(Iterable(type)) MANGLE2(IteratorArray(type), Iterable_inter) = { \
    .iter = CAT(IteratorArray(type), _Iterable_iter), \
}; \
struct VTABLE_TYPE(Iterator(type)) MANGLE2(IteratorArray(type), Iterator_inter) = { \
    .next = CAT(IteratorArray(type), _Iterator_next), \
}; \
struct VTABLE_TYPE(Container(type)) MANGLE2(Array(type), Container_inter) = { \
    .contains = CAT(Array(type), _Container_contains), \
    .is_empty = CAT(Array(type), _Container_is_empty), \
    .len = CAT(Array(type), _Container_len), \
}; \
struct VTABLE_TYPE(Iterable(type)) MANGLE2(Array(type), Iterable_inter) = { \
    .iter = CAT(Array(type), _Iterable_iter), \
}; \
struct VTABLE_TYPE(Sequence(type)) MANGLE2(Array(type), Sequence_inter) = { \
    .get = CAT(Array(type), _Sequence_get), \
    .set = CAT(Array(type), _Sequence_set), \
    .reverse = CAT(Array(type), _Sequence_reverse), \
    . Container(type) = DIST_TO_SUPER_INTERFACE(Array(type), Container(type), Array(type), Sequence(type)), \
    . Iterable(type) = DIST_TO_SUPER_INTERFACE(Array(type), Iterable(type), Array(type), Sequence(type)) \
}; \
type * CAT(IteratorArray(type), _Iterator_next)(INTERFACE_PARAM(Iterator(type))) { \
    DECLARE_SELF(Iterator(type), IteratorArray(type)); \
    if (self->cur == self->stop) { \
        return NULL; \
    } \
    type * out = self->array + self->cur; \
    if (self->step > 0) { \
        if (self->stop - self->cur < self->step) { \
            self->cur = self->stop; \
        } else { \
            self->cur += self->step; \
        } \
    } else { \
        if (self->stop - self->cur > self->step) { \
            self->cur = self->stop; \
        } else { \
            self->cur += self->step; \
        } \
    } \
    return out; \
} \
void CAT(IteratorArray(type), _Iterable_iter)(INTERFACE_PARAM(Iterable(type)), struct Iterable(type) * other) { \
    DECLARE_SELF(Iterable(type), IteratorArray(type)); \
    DECLARE_IMPLEMENTER(Iterable(type), other, IteratorArray(type), dest); \
    *dest = *self; \
    dest->cur = dest->start; \
} \
static const intptr_t CAT(Array(type), _MIN_CAPACITY) = 64 / sizeof(type) ? 64 / sizeof(type) : 8; \
bool CAT(Array(type), _Container_contains)(INTERFACE_PARAM(Container(type)), const type val, int (*comp)(const type *, const type *)) { \
    if (!comp) { \
        return false; \
    } \
    DECLARE_SELF(Container(type), Array(type)); \
    struct IteratorArray(type) iter; \
    ICALL(self, Iterable(type), iter, IGET(&iter, Iterable(type))); \
    for (const type * a = ICALL(&iter, Iterator(type), next); a; a = ICALL(&iter, Iterator(type), next)) { \
        if (!comp(a, &val)) { \
            return true; \
        } \
    } \
    return false; \
} \
bool CAT(Array(type), _Container_is_empty)(INTERFACE_PARAM(Container(type))) { \
    DECLARE_SELF(Container(type), Array(type)); \
    return 0 == self->size; \
} \
intptr_t CAT(Array(type), _Container_len)(INTERFACE_PARAM(Container(type))) { \
    DECLARE_SELF(Container(type), Array(type)); \
    return self->size; \
} \
void CAT(Array(type), _Iterable_iter)(INTERFACE_PARAM(Iterable(type)), struct Iterable(type) * other) { \
    DECLARE_SELF(Iterable(type), Array(type)); \
    DECLARE_IMPLEMENTER(Iterable(type), other, IteratorArray(type), dest); \
    intptr_t start = 0, step = 1, stop = self->size; \
    if (self->reversed) { \
        start = self->size - 1; \
        step = -1; \
        stop = -1; \
    } \
    *dest = (struct IteratorArray(type)){ \
        .array = self->array, \
        .start = start, \
        .cur = start, \
        .stop = stop, \
        .step = step, \
    }; \
} \
type * CAT(Array(type), _Sequence_get)(INTERFACE_PARAM(Sequence(type)), intptr_t loc) { \
    DECLARE_SELF(Sequence(type), Array(type)); \
    if (loc < 0) { \
        if (!self->size) { \
            return NULL; \
        } \
        loc -= ((loc + 1) / self->size - 1) * self->size; \
    } \
    if (loc >= self->size) { \
        return NULL; \
    } \
    loc = self->reversed ? self->size - 1 - loc : loc; \
    return self->array + loc; \
} \
int CAT(Array(type), _Sequence_set)(INTERFACE_PARAM(Sequence(type)), intptr_t loc, type val) { \
    DECLARE_SELF(Sequence(type), Array(type)); \
    if (loc < 0) { \
        if (!self->size) { \
            return 1; \
        } \
        loc -= ((loc + 1) / self->size - 1) * self->size; \
    } \
    if (loc >= self->size) { \
        return 2; \
    } \
    loc = self->reversed ? self->size - 1 - loc : loc; \
    memcpy(self->array + loc, &val, sizeof(type)); \
    return 0; \
} \
void CAT(Array(type), _Sequence_reverse)(INTERFACE_PARAM(Sequence(type))) { \
    DECLARE_SELF(Sequence(type), Array(type)); \
    self->reversed = !self->reversed; \
} \

#endif

