#include "matrix.hpp"
#include "solve.hpp"
#include <iostream>
#include <pthread.h>
#include <math.h>

using namespace std;

void synchronize(int total_threads)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t condvar_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t condvar_out = PTHREAD_COND_INITIALIZER;
    static int threads_in = 0;
    static int threads_out = 0;

    pthread_mutex_lock(&mutex);

    threads_in++;
    if (threads_in >= total_threads)
    {
        threads_out = 0;
        pthread_cond_broadcast(&condvar_in);
    } else
        while (threads_in < total_threads)
            pthread_cond_wait(&condvar_in,&mutex);

    threads_out++;
    if (threads_out >= total_threads)
    {
        threads_in = 0;
        pthread_cond_broadcast(&condvar_out);
    } else
        while (threads_out < total_threads)
            pthread_cond_wait(&condvar_out,&mutex);

    pthread_mutex_unlock(&mutex);
}

void swap_col (double* A, int i, int j, int n) 
{
	double tmp;
	int k;
	for (k = 0; k < n; ++k) 
	{
		tmp = A[i*n+k];
		A[i*n+k] = A[j*n+k];
		A[j*n+k] = tmp;
	}
}
void swap_ind (int* ind, int i, int j) 
{
	int tmp;
	tmp = ind[i];
	ind[i] = ind[j];
	ind[j] = tmp;
}

int solveSystem (int n, double* A, double* X, int* tmp, int thread_num, int total_threads, int* error)
{
	int i, j, k;
	int maxColInd = 0;
	double value;
	int first_row, last_row;

	if (thread_num == 0) {
        for (i = 0; i < n * n; ++i) {
            X[i] = 0;
        }
        for (i = 0; i < n; ++i) {
            X[i * n + i] = 1;
            tmp[i] = i;
        }
    }
        double eps = 1e-100;
	
	
	for (i = 0; i < n; ++i) {
        if (thread_num == 0) {
            value = fabs(A[i * n + i]);
            maxColInd = i;

            for (j = i; j < n; ++j) {
                if (fabs(A[i * n + j]) > value) {
                    value = fabs(A[i * n + j]);
                    maxColInd = j;
                }
            }

            if (fabs(value) < eps) {
                *(error)+= 1;
            }

            if (maxColInd != i) {
                for (j = 0; j < n; ++j) {
                    swap(A[j * n + i], A[j * n + maxColInd]);
                }
                swap_ind(tmp, i, maxColInd);
            }

            value = 1.0 / A[i * n + i];
            if (fabs(value) < eps) {
                *(error)+= 1;
            }

            for (j = i; j < n; ++j) {
                A[i * n + j] *= value;
            }
            for (j = 0; j < n; ++j) {
                X[i * n + j] *= value;
            }
        }
        synchronize(total_threads);

        // PRYAMOI

        first_row = i * thread_num;
        first_row = first_row / total_threads;
        last_row = i * (thread_num + 1);
        last_row = last_row / total_threads;

        for (j = first_row; j < last_row; j++) {
            value = A[j * n + i];
            for (k = i; k < n; k++)
                A[j * n + k] -= value * A[i * n + k];
            for (k = 0; k < n; k++)
                X[j * n + k] -= value * X[i * n + k];
        }
        if (A[n * n - 1] < eps)
            *(error)+=1;
        synchronize(total_threads);
        // OBRATNIY

        first_row = (n - i - 1) * thread_num;
        first_row = first_row / total_threads + i + 1;
        last_row = (n - i - 1) * (thread_num + 1);
        last_row = last_row / total_threads + i + 1;

        for (j = first_row; j < last_row; j++) {
            value = A[j * n + i];
            for (k = i; k < n; k++)
                A[j * n + k] -= value * A[i * n + k];
            for (k = 0; k < n; k++)
                X[j * n + k] -= value * X[i * n + k];
        }
        synchronize(total_threads);
    }
	
	//CHANGES_COLUMNS
	if (thread_num == 0) {
        for (i = 0; i < n;) {
            if (tmp[i] != i) {
                swap_col(X, tmp[i], i, n);
                swap_ind(tmp, tmp[i], i);
            } else
                ++i;
        }
    }
        return 0;
}
