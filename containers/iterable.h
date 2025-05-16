#ifndef ITERATION_H
#define ITERATION_H

#include "utils/macros.h"
#include "utils/interface.h"

typedef enum IteratorStatus {
    ITERATOR_FAIL = -1,
    ITERATOR_CONTINUE,
    ITERATOR_STOP
} IteratorStatus;

#define Iterable(iterator_type) \
MANGLE2(Iterable, iterator_type)

// iterators are also iterable
#define Iterator(type) \
DELIM(Iterator, _, type)

#define DECLARE_Iterable(type) \
struct Iterable(type); \
struct VTABLE_TYPE(Iterable(type)); \
typedef void CAT(Iterable(type), _Iter_f)(INTERFACE_PARAM(Iterable(type)), struct Iterable(type) *); \
DECLARE_INTERFACE_START(Iterable(type)) \
    CAT(Iterable(type), _Iter_f) * iter; \
DECLARE_INTERFACE_END

#define DECLARE_Iterator(type) \
struct Iterator(type); \
struct VTABLE_TYPE(Iterator(type)); \
typedef type * CAT(Iterator(type), _Next_f)(INTERFACE_PARAM(Iterator(type))); \
DECLARE_INTERFACE_START(Iterator(type)) \
    CAT(Iterator(type), _Next_f) * next; \
    struct Iterable(type) * iterable; \
DECLARE_INTERFACE_END

#endif // ITERATION_H
