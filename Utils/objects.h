#define INIT(type, ...) \
DELIM(type, _, init)(__VA_ARGS__)

// first varg must be an identifier
#define AUTO(type, ...) \
struct type INSPECT_0(__VA_ARGS__, UNUSED); \
INIT(type, &__VA_ARGS__)

#define DEST(type, instance_ptr) \
DELIM(type, _, dest)(type, instance_ptr)
