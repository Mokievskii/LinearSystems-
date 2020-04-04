#include "matrix.hpp"
#include <math.h>

using namespace std;

double func (int i, int j) {
	return fabs(i-j);
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
    double error = 0;
    int i, j, k;
    double sum = 0;
    double* T;
    
    T = new double [n*n];
    for (i = 0; i < n*n; ++i)
    {
		T[i] = 0;
	}
    
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j) 
        {
			for (k = 0; k < n; ++k) 
			{ 
				T[i*n + j] += A[i*n + k]*X[k*n + j];
			}
		}
    }
    
	for (i = 0; i < n; ++i) 
	{
		for (j = 0; j < n; ++j) 
		{
			if (j == i) 
			{
				sum += fabs(T[i*n + j] - 1);
			}
			else
			{
				sum += fabs(T[i*n + j]);
			}
		}
		if (sum > error)
		{
			error = sum;
		}
		sum = 0;
	}
	delete []T;
    return error;
}
