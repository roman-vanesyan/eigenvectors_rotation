#include <stdio.h>
#include <math.h>

#include "task_26_10.h"

static
void PrintMatrix(int n, int m, double *A) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            printf("%-15lf", A[i * n + j]);
        }

        printf("\n");
    }

    printf("\n");
}

int evc_26_10(int n, int maxIter, double epsilon, double *A, double *E, double *tmp, double precision) {
    if (n == 1) {
        E[0] = A[0];

        return 0;
    }

    // Точность приближения, т.е. |A|*epsilon
    double accuracy = 1;

    for (int i = 0; i < n; i++) {
        double sum = 0;

        for (int j = 0; j < n; j++) { sum += fabs(A[i * n + j]); }

        if (accuracy < sum) { accuracy = sum; }
    }

    accuracy *= epsilon;

    int iter = 0;

    for (int m = n; m > 2; m--) {
        double *Q = tmp;
        double *R = tmp + n * n;

        while (1) {
            if (iter > maxIter && maxIter > 0) {
                if (IsErrorFlagSet) { printf("Iteration limit is reached!"); }

                return 1;
            }

            iter++;

            if (IsDebugFlagSet) {
                printf("--------------------\n");
                printf("Step: %d\n", iter + 1);
                printf("--------------------\n");
                printf("\n");
            }

            for (int i = 0; i < m; i++) {
                for (int j = 0; j < m; j++) {
                    Q[i * n + j] = 0;

                    // Копируем матрицу A в R
                    R[i * n + j] = A[i * n + j];
                }

                // Единичная матрица для Q
                Q[i * n + i] = 1;
            }

            double s = A[(m - 1) * n + m - 1];
            if (fabs(s) < precision) s += 0.1;

            // A - I*s
            if (IsDebugFlagSet) { printf("Shifting matrix by %lf\n", s); }
            for (int i = 0; i < m; i++) {
                R[i * n + i] -= s;
                if (fabs(R[i * n + i]) < precision) R[i * n + i] = 0;
            }

            for (int i = 0; i < m; i++) {
                for (int j = i + 1; j < m; j++) {
                    double a1 = R[i * n + i];
                    double a2 = R[j * n + i];

                    double z = sqrt((a1 * a1) + (a2 * a2));

                    if (z < precision) {
                        if (IsDebugFlagSet) {
                            printf("Denominator of `c` and `s` equals to 0, skipping...\n");
                        }

                        continue;
                    }

                    double cos = a1 / z;
                    double sin = a2 / z;

                    // Вычисляем матрицу R
                    for (int k = 0; k < m; k++) {
                        a1 = R[i * n + k];
                        a2 = R[j * n + k];

                        R[i * n + k] = cos * a1 + sin * a2;
                        R[j * n + k] = cos * a2 - sin * a1;

                        if (fabs(R[i * n + k]) < precision) { R[i * n + k] = 0; }
                        if (fabs(R[j * n + k]) < precision) { R[j * n + k] = 0; }
                    }

                    // Вычисляем матрицу Q
                    for (int k = 0; k < m; k++) {
                        a1 = Q[k * n + i];
                        a2 = Q[k * n + j];

                        Q[k * n + i] = cos * a1 + sin * a2;
                        Q[k * n + j] = cos * a2 - sin * a1;

                        if (fabs(Q[k * n + i]) < precision) { Q[k * n + i] = 0; }
                        if (fabs(Q[k * n + j]) < precision) { Q[k * n + j] = 0; }
                    }
                }
            }

            if (IsDebugFlagSet) {
                printf("Matrix R:\n");
                PrintMatrix(n, m, R);
                printf("Matrix Q:\n");
                PrintMatrix(n, m, Q);
            }

            // Наивная реализация перемножения матриц за O(n^3).
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < m; j++) {
                    double sum = 0;

                    for (int k = 0; k < m; k++) {
                        sum += R[i * n + k] * Q[k * n + j];
                    }

                    A[i * n + j] = sum;
                    if (fabs(A[i * n + j]) < precision) { A[i * n + j] = 0; }
                }
            }

            // A* + I*s
            if (IsDebugFlagSet) { printf("Unshifting matrix by %lf\n", s); }
            for (int i = 0; i < m; i++) {
                A[i * n + i] += s;
                if (fabs(A[i * n + i]) < precision) A[i * n + i] = 0;
            }

            if (fabs(A[(m - 1) * n + m - 2]) < accuracy) {
                E[m - 1] = A[(m - 1) * n + m - 1];
                if (fabs(E[m - 1]) < precision) E[m - 1] = 0;

                break;
            }

            if (IsDebugFlagSet) {
                printf("Next matrix");
                PrintMatrix(n, m, A);
            }
        }
    }

    double p = -(A[0 * n + 0] + A[1 * n + 1]);
    double q = A[0 * n + 0] * A[1 * n + 1] - A[1 * n + 0] * A[0 * n + 1];
    double d = sqrt(p * p - 4 * q);

    E[0] = (-p + d) / 2;
    E[1] = (-p - d) / 2;

    double t;

    // Сортируем результат в порядке возрастания.
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (E[i] > E[j]) {
                t = E[i];
                E[i] = E[j];
                E[j] = t;
            }
        }
    }

    return 0;
}

int evc_memsize_26_10(int n) {
    return 2 * n * n;
}
