#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <stdio.h>

double func (int i, int j);
int enter_data (double* A, int n, FILE* fin);
void print_res (double* A, int n, int m);
double error_norm (double* A, double* X, int n);

#endif /* MATRIX_HPP */
