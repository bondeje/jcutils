#ifndef PIN_H
#define PIN_H

#ifdef _WIN32

#include <Wsk.h>
#include <stdint.h>

typedef USHORT in_port_t;
typedef u_long in_addr_t;
typedef short sa_family_t;

// for struct in_addr.s_addr access
#define s_addr .S_un.S_addr

#else

#endif

#endif
