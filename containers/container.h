#ifndef CONTAINER_H
#define CONTAINER_H

#include <stdbool.h>
#include <stddef.h>
#include "utils/interface.h"

#define Container(type) \
struct DELIM(Container, _, type);

#define DECLARE_Container(type) \
typedef bool CAT(Container(type), Contains_f)(INTERFACE_PARAM(Container(type)), type); \
typedef bool CAT(Container(type), IsEmpty_f)(INTERFACE_PARAM(Container(type))); \
DECLARE_INTERFACE_START(Container(type)) \
    CAT(Container(type), Contains_f) * contains; \
    CAT(Container(type), IsEmpty_f) * is_empty; \
DECLARE_INTERFACE_END

#endif // CONTAINER_H
