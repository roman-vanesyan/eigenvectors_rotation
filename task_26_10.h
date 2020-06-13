#ifndef EVC_H
#define EVC_H

int IsErrorFlagSet;
int IsDebugFlagSet;

int sim_26_10(int n, double* A, double* tmp, double precision);
int sim_memsize_26_10(int n);

int evc_26_10(int n, int maxIter, double epsilon, double* A, double* E, double* tmp, double precision);
int evc_memsize_26_10(int n);

#endif //EVC_H
