#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "task_26_10.h"

int GetStringLength(const char *string) {
    int i = 0;

    for (; string[i] != 0; i++);

    return i;
}

int CompareStrings(const char *left, const char *right) {
    int lsize = GetStringLength(left);
    int rsize = GetStringLength(right);

    if (lsize != rsize) {
        return 0;
    }

    for (int i = 0; i < lsize; i++) {
        if (left[i] != right[i]) {
            return 0;
        }
    }

    return 1;
}

int ContainsString(const char *left, const char *right) {
    int lsize = GetStringLength(left);
    int rsize = GetStringLength(right);

    if (rsize > lsize) {
        return -1;
    }

    for (int i = 0; i < rsize; i++) {
        if (left[i] != right[i]) {
            return -1;
        }
    }

    return lsize - rsize;
}

void CopyString(const char *src, char **dest, int len) {
    for (int i = 0; i < len; i++) {
        (*dest)[i] = src[i];
    }

    (*dest)[len] = '\0';
}

int IsErrorCode(int code) {
    return code != 0;
}

int ReadMatrix(FILE *in, double **A, int size) {
    if (in == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: error argument, in file is NULL!\n");
        }

        return 100;
    }

    *A = (double *) malloc(size * size * sizeof(double));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (fscanf(in, "%lf", &(*A)[i * size + j]) != 1) {
                return 101;
            }
        }
    }

    return 0;
}

int ReadInput(const char *input, double **A, int *size) {
    FILE *in = fopen(input, "r");
    int code = 0;

    if (in == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to open %s in read mode!\n", input);
        }

        code = 100;

        goto LEND;
    }

    if (fscanf(in, "%d", &(*size)) != 1) {
        code = 101;

        goto LEND;
    }

    code = ReadMatrix(in, A, *size);

    if (IsErrorCode(code)) {
        goto LEND;
    }

    fclose(in);

    LEND:
    if (IsErrorFlagSet && IsErrorCode(code)) {
        printf("ERR: Too few data provided while reading %s!\n", input);
    }

    return code;
}

int WriteOutput(const char *output, int size, double *E) {
    int code = 0;

    FILE *out = fopen(output, "w");
    if (out == NULL) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to open %s in write mode!\n", output);
        }

        code = 100;

        goto RET;
    }

    if (fprintf(out, "%d\n", size) <= 0) {
        code = 102;

        if (IsErrorFlagSet) {
            printf("ERR: Unable to write to file %s!\n", output);
        }

        goto LEND;
    }

    for (int i = 0; i < size; i++) {
        if (fprintf(out, "%1.9lf\n", E[i]) <= 0) {
            code = 102;

            if (IsErrorFlagSet) {
                printf("ERR: Unable to write to file %s!\n", output);
            }

            break;
        }
    }

    LEND:
    fclose(out);

    RET:
    return code;
}

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

void PrintHelpMessage() {
    printf("./evc$ ./evc_26_10 -?\n");
    printf(" Usage: evc [input_file_name] [output_file_name] [options]\n");
    printf("Where options include:\n");
    printf("-d                print debug messages [default OFF]\n");
    printf("-e                print errors [default OFF]\n");
    printf("-p                print matrix [default OFF]\n");
    printf("-t                print execution time [default OFF]\n");
    printf("-prec=<num>       precision [default - 1e-14]\n");
    printf("-eps=<num>        epsilon [default - 1e-10]\n");
    printf("-max_iter=<num>   limit number of iterations [default - 0, i.e. not limit]\n");
    printf("-h, -?            print this and exit\n");
}

int ReadCommandLineArguments(
        int count,
        char *arguments[],
        int *isHelpFlagSet,
        int *isShowMatrixFlagSet,
        int *isTraceFlagSet,
        int *maxIter,
        double *eps,
        double *precision,
        char **input,
        char **output
) {
    int code = 0;
    int isInputSet = 0;
    int isOutputSet = 0;
    int len;

    for (int i = 1; i < count; i++) {
        char *arg = arguments[i];

        if (CompareStrings(arg, "-h") || CompareStrings(arg, "-?")) {
            *isHelpFlagSet = 1;
        } else if (CompareStrings(arg, "-d")) {
            IsDebugFlagSet = 1;
        } else if (CompareStrings(arg, "-e")) {
            IsErrorFlagSet = 1;
        } else if (CompareStrings(arg, "-t")) {
            *isTraceFlagSet = 1;
        } else if (CompareStrings(arg, "-p")) {
            *isShowMatrixFlagSet = 1;
        } else if ((len = ContainsString(arg, "-prec=")) > 0) {
            char *num = (char *) malloc(len * sizeof(char));

            CopyString(arg + 6, &num, len);

            if (sscanf(num, "%lf", &(*precision)) == 0) {
                free(num);

                code = 200;
                goto RET;
            }

            free(num);
        } else if ((len = ContainsString(arg, "-max_iter=")) > 0) {
            char *num = (char *) malloc(len * sizeof(char));

            CopyString(arg + 10, &num, len);

            if (sscanf(num, "%d", &(*maxIter)) == 0) {
                free(num);

                code = 200;
                goto RET;
            }

            free(num);
        } else if ((len = ContainsString(arg, "-eps=")) > 0) {
            char *num = (char *) malloc(len * sizeof(char));

            CopyString(arg + 5, &num, len);

            if (sscanf(num, "%lf", &(*eps)) == 0) {
                free(num);

                code = 200;
                goto RET;
            }

            free(num);
        } else if (arg[0] == '-') {
            code = 202;

            goto RET;
        } else {
            if (*isHelpFlagSet ||
                IsDebugFlagSet ||
                IsErrorFlagSet ||
                *isTraceFlagSet ||
                *isShowMatrixFlagSet) {
                code = 200;

                goto RET;
            } else if (!isInputSet) {
                *input = arg;
                isInputSet = 1;
            } else if (!isOutputSet) {
                *output = arg;
                isOutputSet = 1;
            } else {
                code = 201;

                goto RET;
            }
        }
    }

    RET:
    return code;
}

int main(int argc, const char *argv[]) {
    int code = 0;

    char *input = "26_10_in.txt";
    char *output = "26_10_out.txt";

    int isHelpFlagSet = 0;
    int isShowMatrixFlagSet = 0;
    int isTraceFlagSet = 0;
    double precision = 1e-14;
    double epsilon = 1e-12;
    int maxIter = 0;
    IsErrorFlagSet = 0;
    IsDebugFlagSet = 0;

    code = ReadCommandLineArguments(
            argc,
            (char **) argv,
            &isHelpFlagSet,
            &isShowMatrixFlagSet,
            &isTraceFlagSet,
            &maxIter,
            &epsilon,
            &precision,
            &input,
            &output
    );

    if (IsDebugFlagSet) {
        printf("eps=%lf | precision=%lf | max_iter=%d\n", epsilon, precision, maxIter);
    }

    if (IsErrorCode(code)) {
        goto RET;
    }

    if (isHelpFlagSet) {
        PrintHelpMessage();

        goto RET;
    }

    int size;
    double *A = NULL;
    double *E = NULL;
    double *tmp = NULL;

    code = ReadInput(input, &A, &size);

    if (IsErrorCode(code)) {
        goto LEND;
    }

    if (isShowMatrixFlagSet) {
        PrintMatrix(size, A);
    }

    E = (double *) malloc(size * sizeof(double));
    tmp = (double *) malloc(sim_memsize_26_10(size) * sizeof(double));

    time_t start = clock();
    code = sim_26_10(size, A, tmp, precision);

    if (IsErrorCode(code)) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to simplify given matrix A!\n");
        }

        size = 0;
        goto WEND;
    }

    free(tmp);
    tmp = (double *) malloc(evc_memsize_26_10(size) * sizeof(double));

    code = evc_26_10(size, maxIter, epsilon, A, E, tmp, precision);

    if (IsErrorCode(code)) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to find eigenvalue for given matrix A!");
        }

        size = 0;
        goto WEND;
    }

    double duration = (double) (clock() - start) / CLOCKS_PER_SEC;

    if (isTraceFlagSet) {
        printf("TRACE: the result was computed in %lfsec.\n", duration);
    }

    WEND:
    code = WriteOutput(output, size, E);

    if (IsErrorCode(code)) {
        if (IsErrorFlagSet) {
            printf("ERR: Unable to write to output file!");
        }
    }

    LEND:
    free(A);
    free(E);
    free(tmp);

    A = NULL;
    E = NULL;
    tmp = NULL;

    RET:
    return code;
}
