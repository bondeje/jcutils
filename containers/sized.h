#ifndef SIZED_H
#define SIZED_H

#include <stddef.h>
#include "utils/interface.h"

struct Sized;

typedef size_t SizedLen_f(INTERFACE_PARAM(Sized));

DECLARE_INTERFACE_START(Sized)
    SizedLen_f * len;
DECLARE_INTERFACE_END

#endif // SIZED_H
