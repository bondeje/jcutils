#ifndef STRINGIFY
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#endif
#ifndef CAT
#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)
#endif
#ifndef CAT3
#define CAT3_(a, b, c) a##b##c
#define CAT3(a, b, c) CAT3_(a, b, c)
#endif

#define TEST_START(verbosity) (verbosity ? printf("%s...", __func__) : 0)
#define TEST_END(verbosity, nerrors) (verbosity ? (nerrors ? printf("failed with %d errors\n", nerrors) : printf("passed\n")) : 0)

#define CHECK_VA_ARGS(result, failmsg, ...) (result ? 0 : printf("\n\t%s-%s-%d: " failmsg "\n", __FILE__, __func__, __LINE__, __VA_ARGS__))
#define CHECK_2(result, failmsg) (result ? 0 : printf("\n\t%s-%s-%d: " failmsg "\n", __FILE__, __func__, __LINE__))
#define CHECK_1(result) CHECK_2(result, "")

#define GET_CHECK_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, Name, ...) Name
#define CHECK(...) GET_CHECK_MACRO(__VA_ARGS__, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_VA_ARGS, CHECK_2, CHECK_1, UNUSED)(__VA_ARGS__)
