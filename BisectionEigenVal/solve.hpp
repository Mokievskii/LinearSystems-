#ifndef SOLVE_HPP
#define SOLVE_HPP

void To3D (int n, double* A);
double Norm(int n, double *A);
void EV_k (int n, double *A, int k, double EPS, double *value, int *count_iter);
int change (int n, double *A, double la);

#endif /* SOLVE_HPP */
