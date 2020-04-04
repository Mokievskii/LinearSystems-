#include "matrix.hpp"
#include <math.h>
#include <sys/resource.h>
#include <sys/time.h>

using namespace std;

double func (int i, int j) {
	return fabs(i - j);
}

int enter_data (double* A, int n, FILE* fin) {
	int i, j;
	if (fin) {
		for (i = 0; i < n; ++i)
        {
            for (j = 0; j < n; ++j)
            {
                if (fscanf(fin, "%lf", &A[i*n+j]) != 1)
                    return -1;
            }
        }
	}
	else {
		for (i = 0; i < n; ++i)
        {
            for (j = 0; j < n; ++j)
            {
                A[i*n+j] = func(i, j);
            }
        }
	}
	
	return 0;
}

void print_res (double* A, int n, int m)
{
    int i, j;
    int min_ = fmin(n, m);
    
    for (i = 0; i < min_; ++i)
        {
            for (j = 0; j < min_; ++j)
            {
                printf("%8.5f ", A[i*n + j]);
            }
            printf("\n");
        }
    printf("\n");
}

double error_norm(double* A, double* X, int n)
{
    int i, j, k;
    double tmp;
    double norm;

    norm = 0.0;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            tmp = 0.0;
            for (k = 0; k < n; k++)
                tmp += A[i * n + k] * X[k * n + j];
            if (i == j)
                tmp -= 1.0;
            norm += tmp * tmp;
        }

    return sqrt(norm);
}
