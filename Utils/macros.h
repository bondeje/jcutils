#ifndef CAT
#define CAT_(x, y) x##y
#define CAT(x, y) CAT_(x, y)
#endif

#ifndef DELIM
#define DELIM_(x, delim, y) x##delim##y
#define DELIM(x, delim, y) DELIM_(x, delim, y)
#endif

#define MANGLE2_(x, y) x##_##y
#define MANGLE2(x, y) MANGLE2_(x, y)
#define MANGLE3_(x, y, z) x##_##y##_##z
#define MANGLE3(x, y, z) MANGLE3_(x, y, z)

#ifndef STRINGIFY
#define STRINGIFY_(x, y) #x
#define STRINGIFY(x, y) STRINGIFY_(x, y)
#endif

#define REMOVE_UNUSED_UNUSED
#define REMOVE_UNUSED(A) DELIM(REMOVE_UNUSED, _, A)
#define INSPECT_0(A, ...) A
#define IGNORE_0(A, ...) REMOVE_UNUSED(UNUSED __VA_ARGS__)
