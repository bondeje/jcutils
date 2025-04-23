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
struct MANGLE2(Iterable, iterator_type)

#define DECLARE_Iterable(iterator_type) \
typedef IteratorStatus CAT(Iterable(iterator_type), Iter_f)(INTERFACE_PARAM(Iterable(iterator_type)), iterator_type *); \
DECLARE_INTERFACE_START(Iterable(iterator_type)) \
    CAT(Iterable(iterator_type), Iter_f) * iter; \
DECLARE_INTERFACE_END

// iterators are also iterable
#define Iterator(type)
struct DELIM(Iterator, _, type);

#define DECLARE_Iterator(type) \
typedef IteratorStatus CAT(Iterator(type), Next_f)(INTERFACE_PARAM(Iterator(type)), type *); \
DECLARE_INTERFACE_START(Iterator(type)) \
    CAT(Iterator(type), Next_f) * next; \
DECLARE_INTERFACE_END

#endif // ITERATION_H
