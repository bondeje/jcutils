#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>

#define DEFAULT_NLOOPS 16384
#define DEFAULT_NVALUES 1024

long nloops = DEFAULT_NLOOPS;
int nvalues = DEFAULT_NVALUES;

#if !defined(__STDC_NO_COMPLEX__)
#include <complex.h>

void test_complex_c(void) {
    double _Complex values[DEFAULT_NVALUES];
    double time = 0.0;
    struct timespec t0, t1;
    clockid_t clk = CLOCK_MONOTONIC;

    for (int i = 0; i < DEFAULT_NVALUES; i++) {
        values[i] = rand() * 1.0 / RAND_MAX + rand() * I / RAND_MAX;
        double mag = sqrt(pow(creal(values[i]), 2) + pow(cimag(values[i]), 2));
        values[i] /= mag;
    }
    
    double _Complex val = rand() * 1.0 / RAND_MAX + rand() * I / RAND_MAX;
    double mag = sqrt(pow(creal(val), 2) + pow(cimag(val), 2));
    val /= mag;

    // timing start
    clock_gettime(clk, &t0);
    for (int i = 0; i < nloops; i++) {
        for (int j = 0; j < nvalues; j++) {
            val *= values[j];
        }
    }
    // timing end
    clock_gettime(clk, &t1);

    if (t1.tv_nsec < t0.tv_nsec) {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    } else {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    }

    printf("%s - final value: %f + %fi. time = %f sec\n", __func__, creal(val), cimag(val), time);
}

#endif

typedef double complex2d[2];

void test_complex_2d(void) {
    complex2d values[DEFAULT_NVALUES];
    double time = 0.0;
    struct timespec t0, t1;
    clockid_t clk = CLOCK_MONOTONIC;

    for (int i = 0; i < DEFAULT_NVALUES; i++) {
        values[i][0] = rand() * 1.0 / RAND_MAX;
        values[i][1] = rand() * 1.0 / RAND_MAX;
        double mag = sqrt(pow(values[i][0], 2) + pow(values[i][1], 2));
        values[i][0] /= mag;
        values[i][1] /= mag;
    }
    
    complex2d val = {rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX};
    double mag = sqrt(pow(val[0], 2) + pow(val[1], 2));
    val[0] /= mag;
    val[1] /= mag;

    // timing start
    clock_gettime(clk, &t0);
    for (int i = 0; i < nloops; i++) {
        for (int j = 0; j < nvalues; j++) {
            double r = val[0] * values[j][0] - val[1] * values[j][1];
            double c = val[1] * values[j][0] + val[0] * values[j][1];
            val[0] = r;
            val[1] = c;
        }
    }
    // timing end
    clock_gettime(clk, &t1);

    if (t1.tv_nsec < t0.tv_nsec) {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    } else {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    }

    printf("%s - final value: %f + %fi. time = %f sec\n", __func__, val[0], val[1], time);
}

void test_complex_2dT(void) {
    double valuesarr[2 * DEFAULT_NVALUES];
    double time = 0.0;
    struct timespec t0, t1;
    clockid_t clk = CLOCK_MONOTONIC;

    for (int i = 0; i < 2 * DEFAULT_NVALUES; i+=2) {
        valuesarr[i] = rand() * 1.0 / RAND_MAX;
        valuesarr[i + 1] = rand() * 1.0 / RAND_MAX;
        double mag = sqrt(pow(valuesarr[i], 2) + pow(valuesarr[i + 1], 2));
        valuesarr[i] /= mag;
        valuesarr[i + 1] /= mag;
    }

    complex2d * values = (complex2d *)&valuesarr[0];
    
    complex2d val = {rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX};
    double mag = sqrt(pow(val[0], 2) + pow(val[1], 2));
    val[0] /= mag;
    val[1] /= mag;

    //complex2d valarr = val;

    // timing start
    clock_gettime(clk, &t0);
    for (int i = 0; i < nloops; i++) {
        for (int j = 0; j < nvalues; j++) {
            double r = val[0] * values[j][0] - val[1] * values[j][1];
            double c = val[1] * values[j][0] + val[0] * values[j][1];
            val[0] = r;
            val[1] = c;
        }
    }
    // timing end
    clock_gettime(clk, &t1);

    if (t1.tv_nsec < t0.tv_nsec) {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    } else {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    }

    printf("%s - final value: %f + %fi. time = %f sec\n", __func__, val[0], val[1], time);
}

typedef struct {
    double r;
    double i;
} complexs;

void test_complex_s(void) {
    complexs values[DEFAULT_NVALUES];
    double time = 0.0;
    struct timespec t0, t1;
    clockid_t clk = CLOCK_MONOTONIC;

    for (int i = 0; i < DEFAULT_NVALUES; i++) {
        values[i].r = rand() * 1.0 / RAND_MAX;
        values[i].i = rand() * 1.0 / RAND_MAX;
        double mag = sqrt(pow(values[i].r, 2) + pow(values[i].i, 2));
        values[i].r /= mag;
        values[i].i /= mag;
    }
    
    complexs val = {rand() * 1.0 / RAND_MAX, rand() * 1.0 / RAND_MAX};
    double mag = sqrt(pow(val.r, 2) + pow(val.i, 2));
    val.r /= mag;
    val.i /= mag;

    // timing start
    clock_gettime(clk, &t0);
    for (int i = 0; i < nloops; i++) {
        for (int j = 0; j < nvalues; j++) {
            double r = val.r * values[j].r - val.i * values[j].i;
            double c = val.i * values[j].r + val.r * values[j].i;
            val.r = r;
            val.i = c;
        }
    }
    // timing end
    clock_gettime(clk, &t1);

    if (t1.tv_nsec < t0.tv_nsec) {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    } else {
        time += ((1 + 1.0e-9 * t1.tv_nsec) - 1.0e-9 * t0.tv_nsec) + t1.tv_sec - 1.0 - t0.tv_sec;
    }

    printf("%s - final value: %f + %fi. time = %f sec\n", __func__, val.r, val.i, time);
}

int main(int argc, char ** args) {
    srand(0);

    if (argc > 1) {
        printf("%s\n", args[1]);
        nloops = strtol(args[1], NULL, 10);
    }
    printf("nloops = %ld\n", nloops);

#if !defined(__STDC_NO_COMPLEX)
    test_complex_c();
    test_complex_2d();
    test_complex_s();
#endif
    return 0;
}