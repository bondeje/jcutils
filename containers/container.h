#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "utils/interface.h"

#define Container(type) \
DELIM(Container, _, type)

#define DECLARE_Container(type) \
struct Container(type); \
struct VTABLE_TYPE(Container(type)); \
typedef bool CAT(Container(type), _Contains_f)(INTERFACE_PARAM(Container(type)), const type, int (*)(const type *, const type *)); \
typedef bool CAT(Container(type), _IsEmpty_f)(INTERFACE_PARAM(Container(type))); \
typedef intptr_t CAT(Container(type), _Len_f)(INTERFACE_PARAM(Container(type))); \
DECLARE_INTERFACE_START(Container(type)) \
    CAT(Container(type), _Contains_f) * contains; \
    CAT(Container(type), _IsEmpty_f) * is_empty; \
    CAT(Container(type), _Len_f) * len; \
DECLARE_INTERFACE_END

#endif // CONTAINER_H
