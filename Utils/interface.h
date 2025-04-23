#include <stddef.h>
#include "objects.h"

#define IGET(instance_ptr, interface) \
((instance_ptr)->interface)

#define ICALL(instance_ptr, interface, method, ...) \
(IGET(instance_ptr, interface).inter)->method(&IGET(instance_ptr, interface), __VA_ARGS__)

#define IGETS(instance_ptr, super, interface) \
(((super *)(instance_ptr))->interface)

#define ICALLS(instance_ptr, super, interface, method, ...) \
(IGETS(instance_ptr, super, interface).inter)->method(&IGETS(instance_ptr, super, interface), __VA_ARGS__)

#define GET_IMPLEMENTATION(interface, implementation_type) \
DELIM(interface, _, implementation_type)

#define ISET(instance_ptr, implementation_type, interface) \
IGET(instance_ptr, interface) = GET_IMPLEMENTATION(interface, implementation_typ)

#define ISETS(instance_ptr, super, implementation_type, interface) \
IGETS(instance_ptr, interface) = GET_IMPLEMENTATION(interface, implementation_typ)

#define INTERFACE_PARAM(interface_type) \
struct interface_type const * __restrict__ interface_type

#define IMPLEMENTS(interface_type) \
struct interface_type interface_type

#define DECLARE_SELF(interface_type, implementation_type) \
struct implementation_type * self = (struct implementation_type *)((char *)interface_type - offsetof(struct implementation_type, interface_type))

#define DECLARE_INTERFACE_START(interface_name) \
struct interface_name { \
    struct CAT(interface_name, _inter) * inter; \
}; \
struct CAT(interface_name, _inter) { \

#define DECLARE_INTERFACE_END };

#define DECLARE_IMPLEMENTATION(interface, implementation_type) \
extern struct interface GET_IMPLEMENTATION(interface, implementation_type)

#define INTERFACE_INITIALIZER_START(interface) { \
    .inter = &(struct CAT(interface, _inter)) { \

#define INTERFACE_INITIALIZER_END }}

#define DEFINE_IMPLEMENTATION_START(interface, implementation_type) \
struct interface GET_IMPLEMENTATION(interface, implementation_type) = INTERFACE_INITIALIZER_START(interface)


#define DEFINE_IMPLEMENTATION_END INTERFACE_INITIALIZE_END};
