#include "matrix.hpp"
#include "solve.hpp"
#include <iostream>
#include <math.h>

using namespace std;

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

int solveSystem (int n, double* A, double* X, int* tmp) 
{
	int i, j, k;
	int maxColInd = 0;
	double value;
	
	for (i = 0; i < n*n; ++i) 
	{
		X[i] = 0;
	}
	for (i = 0; i < n; ++i) 
	{
		X[i*n + i] = 1;
		tmp[i] = i;
	}
	
        double eps = 1e-100;
	
	//PRYAMOI 
	
	
	for (i = 0; i < n; ++i)
    {
        value = fabs(A[i*n+i]);
        maxColInd = i;
        
        for (j = i; j < n; ++j)
        {
                if (fabs(A[i*n + j]) > value)
                {
                    value = fabs(A[i*n + j]);
                    maxColInd = j;
                }
        }
        
        if (fabs(value) < eps)
        {
            return -1;
        }
        
        if (maxColInd != i)
        {
			for (j = 0; j < n; ++j) 
			{
				swap(A[j*n + i], A[j*n + maxColInd]);
			}
			swap_ind(tmp, i, maxColInd);
        }
        
        value = 1.0/A[i*n+i];
        if (fabs(value) < eps)
        {
            return -1;
        }

        for (j = i; j < n; ++j)
        {
            A[i*n+j] *= value;  
        }
        for (j = 0; j < n; ++j) 
        {
			X[i*n+j] *= value;
		}
        for(j = 0; j < n; ++j)
        {
            if (j != i)
            {
                value = A[j*n+i];
                
                for(k = i; k < n; ++k)
                {
                    A[j*n+k] -= value*A[i*n+k];
                }
                for(k = 0; k < n; ++k)
                {
                    X[j*n+k] -= value*X[i*n+k];
                }
            }
        }
        if (A[n*n - 1] < eps) 
			return -1;
    }
    
    //OBRATNI


    for (i = n-1; i > 0; --i)
    {
		for(j = i-1; j >= 0; --j)
			{
				if (j != i)
				{
					value = A[j*n+i];
                
					for(k = 0; k < n; ++k)
					{
						A[j*n+k] -= value*A[i*n+k];
						X[j*n+k] -= value*X[i*n+k];
					}
				}
			}
	}
	
	//CHANGES_COLUMNS
	
    for (i = 0; i < n; ) 
    {
		if (tmp[i] != i) 
		{
			swap_col(X, tmp[i], i, n);
			swap_ind(tmp, tmp[i], i);
		}
		else
			++i;
	}
	return 0;
}
