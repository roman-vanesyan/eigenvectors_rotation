#include <stdio.h>
#include <math.h>

#include "task_26_10.h"

static
void PrintMatrix(int size, double *A) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%-15lf", A[i * size + j]);
        }

        printf("\n");
    }

    printf("\n");
}

int sim_26_10(int n, double *A, double *tmp, double precision) {
    double a1;
    double a2;
    double c;
    double s;
    double z;

    for (int i = 1; i < n; i++) {
        int step = i;

        if (IsDebugFlagSet && step != n) {
            printf("--------------------\n");
            printf("Step: %d\n", step);
            printf("--------------------\n");
            printf("\n");
        }

        for (int j = i + 1; j < n; j++) {
            a1 = A[i * n + i - 1];
            a2 = A[j * n + i - 1];
            z = sqrt((a1 * a1) + (a2 * a2));

            if (fabs(z) < precision) {
                if (IsDebugFlagSet) {
                    printf("Denominator of `c` and `s` equals to 0, skip step: %d\n", step);
                }

                continue;
            }

            c = a1 / z;
            s = a2 / z;

            for (int k = 0; k < n; k++) {
                a1 = A[i * n + k];
                a2 = A[j * n + k];

                A[i * n + k] = c * a1 + s * a2;
                A[j * n + k] = c * a2 - s * a1;

                if (fabs(A[i * n + k]) < precision) { A[i * n + k] = 0; }
                if (fabs(A[j * n + k]) < precision) { A[j * n + k] = 0; }
            }

            for (int k = 0; k < n; k++) {
                a1 = A[k * n + i];
                a2 = A[k * n + j];

                A[k * n + i] = c * a1 + s * a2;
                A[k * n + j] = c * a2 - s * a1;

                if (fabs(A[k * n + i]) < precision) { A[k * n + i] = 0; }
                if (fabs(A[k * n + j]) < precision) { A[k * n + j] = 0; }
            }

            if (IsDebugFlagSet) {
                PrintMatrix(n, A);
            }
        }
    }

    if (IsDebugFlagSet) {
        printf("Simplification done!\n");
        PrintMatrix(n, A);
    }

    return 0;
}

int sim_memsize_26_10(int n) {
    return 0;
}
