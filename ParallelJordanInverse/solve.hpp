#ifndef SOLVE_HPP
#define SOLVE_HPP

void synchronize(int total_threads);
void swap_ind (int* ind, int i, int j);
void swap_col (double* A, int i, int j, int n);
int solveSystem(int n, double* A, double* X, int* tmp, int thread_num, int total_threads, int* error);


#endif /* SOLVE_HPP */
