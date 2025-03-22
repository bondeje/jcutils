#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#include "fft.c"
#include "../test/utils.h"

//size_t MIN_LENGTH = 3;
//size_t MAX_LENGTH = 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3;

size_t MIN_LENGTH = 2;
size_t MAX_LENGTH = 8192;

double rms_error(dcomplex * result, dcomplex * src, size_t n) {
    double res = 0.0;
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        dcomplex delta = csub(result[i], src[i]);
        res += pow(creal(delta), 2) + pow(cimag(delta), 2);
        sum += pow(creal(src[i]), 2) + pow(cimag(src[i]), 2);
    }
    return sqrt(res / sum);
}

void posix_time(void) {
    TEST_START(1);

    int nerrors = 0;

    double errsum = 0.0;

    double time = 0.0;
    LARGE_INTEGER t0;
    LARGE_INTEGER t1;
    LARGE_INTEGER clock_freq;
    QueryPerformanceFrequency(&clock_freq);
    double clock_conv = 1.0 / clock_freq.QuadPart;

    dcomplex * data = malloc(MAX_LENGTH * sizeof(dcomplex));
    for (size_t i = 0; i < MAX_LENGTH; i++) {
        data[i] = (dcomplex)dcomplex((rand()*2.0 - 1)/RAND_MAX, (rand()*2.0 - 1)/RAND_MAX);
    }
    dcomplex * ft = malloc(MAX_LENGTH * sizeof(dcomplex));
    dcomplex * ift = malloc(MAX_LENGTH * sizeof(dcomplex));
    //size_t size = cdft_w(ft, data, MAX_LENGTH, DFT_FORWARD | DFT_NORM_INV, NULL);
    //void * ws = malloc(size);

    for (size_t length = MIN_LENGTH; length < MAX_LENGTH; length *= 2) {
        printf("iteration %zu\n", length);

        // timing start
        QueryPerformanceCounter(&t0);
        //size_t status = cdft_w(ft, data, length, DFT_FORWARD | DFT_NORM_INV, ws);
        size_t status = cdft(ft, data, length, DFT_FORWARD | DFT_NORM_INV);
        if (status) {
            printf("forward failed with status %zu\n", status);
            nerrors++;
            continue;
        }

        //status = cdft_w(ift, ft, length, DFT_INVERSE | DFT_NORM_INV, ws);
        status = cdft(ift, ft, length, DFT_INVERSE | DFT_NORM_INV);
        if (status) {
            printf("inverse failed with status %zu\n", status);
            nerrors++;
            continue;
        }

        // timing end
        QueryPerformanceCounter(&t1);
        time += (t1.QuadPart - t0.QuadPart) * clock_conv;

        double error = rms_error(ift, data, length);
        /*printf("error: %e - ", error);
        for (size_t i = 0; i < length; i++) {
            print_complex(data[i], " ", " - ");
            print_complex(ift[i], "", " ");
        }*/
        if (error > 2e-10) {
            printf("problem at complex length %zu: %e\n", length, error);
        }
        errsum += error;
    }

    printf("errsum: %e. Time (s): %f\n", errsum, time);

    TEST_END(1, nerrors);
}

int main(int narg, char ** args) {
    posix_time();
    return 0;
}