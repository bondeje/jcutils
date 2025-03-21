#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <complex.h>

#ifndef CAT
#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)
#endif

#define dcomplex_select(_2, _1, NAME, ...) NAME
#define dcomplex(...) dcomplex_select(__VA_ARGS__, dcomplex_2, dcomplex_1, UNUSED)(__VA_ARGS__)

#if defined(__STDC_NO_COMPLEX__)

#define dcomplex_1(a) {a, 0.0}
#define dcomplex_2(a, b) {a, b}

#if defined(_MSC_VER) /* MSVC with complex _#complex */

typedef _Dcomplex dcomplex;
static const dcomplex dcomplex_I = {._Val = {0.0, 1.0}};
#define cadd(a, b) (dcomplex){._Val = {(a)._Val[0] + (b)._Val[0], (a)._Val[1] + (b)._Val[1]}}
#define caddr(a, real) (dcomplex){._Val = {(a)._Val[0] + real, (a)._Val[1]}}
#define csub(a, b) (dcomplex){._Val = {(a)._Val[0] - (b)._Val[0], (a)._Val[1] - (b)._Val[1]}}
#define cdiv(a, b) (dcomplex){._Val = {((a)._Val[0] * (b)._Val[0] + (a)._Val[1] * (b)._Val[1]) / ((b)._Val[0]* (b)._Val[0] + (b)._Val[1] * (b)._Val[1]), ((a)._Val[1] * (b)._Val[0] - (a)._Val[0] * (b)._Val[1]) / ((b)._Val[0] * (b)._Val[0] + (b)._Val[1] * (b)._Val[1])}}
#define cmul(a, b) _Cmulcc(a, b)
#define cmulr(a, real) _Cmulcr(a, real)
#define ceq(a, b) (((a)._Val[0] == (b)._Val[0]) && ((a)._Val[1] == (b)._Val[1]))

#else

#define dcomplex_1(a) {.r = a, .i = 0.0}
#define dcomplex_2(a, b) {.r = a, .i = b}

typedef struct {
    double r;
    double i;
} dcomplex;
static const dcomplex dcomplex_I = {.r = 0.0, .i = 1.0};
#define creal(dcmplx) (dcmplx).r
#define cimag(dcmplx) (dcmplx).i
#define cadd(a, b) (dcomplex){.r = (a).r + (b).r, .i = (a).i + (b).i}
#define caddr(a, b) (dcomplex){.r = (a).r + b, .i = (a).i}
#define csub(a, b) (dcomplex){.r = (a).r - (b).r, .i = (a).i - (b).i}
#define cdiv(a, b) (dcomplex){.r = ((a).r * (b).r + (a).i * (b).i) / ((b).r * (b).r + (b).i * (b).i), .i = ((a).i * (b).r - (a).r * (b).i) / ((b).r * (b).r + (b).i * (b).i)}
#define cmul(a, b) (dcomplex){.r = (a).r * (b).r - (a).i * (b).i, .i = (a).r * (b).i + (a).i * (b).r}
#define cmulr(a, real) (dcomplex){.r = (a).r * real, .i = (a).i * real}
#define norm(a) ((a).r * (a).r + (a).i * (a).i)
#define ceq(a, b) (((a).r == (b).r) && ((a).i == (b).i))

#endif

#else

/* for testing purposes
#define dcomplex_1(a) {.r = a, .i = 0.0}
#define dcomplex_2(a, b) {.r = a, .i = b}

// recommend compiling with -fcx-limited-range to eliminate checks for NaN and range extensions
typedef struct {
    double r;
    double i;
} dcomplex;
static const dcomplex dcomplex_I = {.r = 0.0, .i = 1.0};
#define creal(dcmplx) (dcmplx).r
#define cimag(dcmplx) (dcmplx).i
#define cadd(a, b) (dcomplex){.r = (a).r + (b).r, .i = (a).i + (b).i}
#define caddr(a, b) (dcomplex){.r = (a).r + b, .i = (a).i}
#define csub(a, b) (dcomplex){.r = (a).r - (b).r, .i = (a).i - (b).i}
#define cdiv(a, b) (dcomplex){.r = ((a).r * (b).r + (a).i * (b).i) / ((b).r * (b).r + (b).i * (b).i), .i = ((a).i * (b).r - (a).r * (b).i) / ((b).r * (b).r + (b).i * (b).i)}
#define cmul(a, b) (dcomplex){.r = (a).r * (b).r - (a).i * (b).i, .i = (a).r * (b).i + (a).i * (b).r}
#define cmulr(a, real) (dcomplex){.r = (a).r * real, .i = (a).i * real}
#define norm(a) ((a).r * (a).r + (a).i * (a).i)
#define ceq(a, b) (((a).r == (b).r) && ((a).i == (b).i))
#define cexp(a) (dcomplex){.r = cos(cimag((a))), .i = sin(cimag((a)))}
*/

// built-in complex data type 
#define dcomplex_1(a) a
#define dcomplex_2(a, b) a + dcomplex_I * b

typedef double _Complex dcomplex;
static const dcomplex dcomplex_I = (dcomplex)_Complex_I;
#define edtoc(a) a
#define cadd(a, b) (a + b)
#define caddr(a, b) (a + b)
#define csub(a, b) (a - b)
#define cdiv(a, b) (a / b)
#define cmul(a, b) (a * b)
#define cmulr(a, real) (a * real)
#define ceq(a, b) (a == b)

#endif

/*
static inline void dbl_to_complex(dcomplex * out, double * in, size_t n) {
    for (size_t i = 0; i < n; i++) {
        out[i] = (dcomplex){in[i]};
    }
}
*/

#endif