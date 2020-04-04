#include "matrix.hpp"
#include "solve.hpp"
#include <iostream>
#include <math.h>

using namespace std;

void To3D (int n, double* A)
{
	int i, j, k;
	double x1, x2;
	
	for (i = 0; i < n - 2; ++i) 
	{
		x1 = 0.0;
		x2 = 0.0;
		
		//calc ||a_i||
		
		for (j = i + 1; j < n; ++j)
		{
			x1 += A[j*n + i] * A[j*n + i];
		}
		x2 = sqrt(x1);
		
		if (x2 < 1e-20)
		{
			A[(i+1)*n + i] = 0.0;
			A[(i+2)*n + i] = 0.0;
			continue;
		}
		
		if (sqrt(x1 -  A[(i+1)*n + i]*A[(i+1)*n + i]) < 1e-20)
		{
			A[(i+2)*n + i] = 0.0;
			continue;
		}
		
		//calc x_i = (a_i - e_i*||a_i||) / || a_i - e_i*||a_i|| ||
		
		x1 -=  A[(i+1)*n + i]*A[(i+1)*n + i];
		A[(i+1)*n + i] -= x2;
		x1 = 1.0 / sqrt(x1 + A[(i+1)*n + i]*A[(i+1)*n + i]);
		for (j = i + 1; j < n; ++j)
		{
			A[j*n + i] *= x1;
		}
		
		// calc y = Ax
		
		for (j = i + 1; j < n; ++j)
		{
			x1 = 0.0;
			for (k = i + 1; k < n; ++k)
				x1 += A[j * n + k] * A[k * n + i];
			A[i * n + j] = x1;
		}
		
		//calc (x, y)
		
		x1 = 0.0;
		for (j = i + 1; j < n; ++j)
		{
			x1 += A[i * n + j] * A[j * n + i];
		}
		
		//calc z = 2*y - 2*(x,y)x
		
		for (j = i + 1; j < n; ++j)
		{
			A[i * n + j] = A[i * n + j] - x1 * A[j * n + i];
			A[i * n + j] *= 2.0;
		}
		
		//calc B = A - zx - xz
		
		for (j = i + 1; j < n; ++j)
			for (k = i + 1; k < n; ++k)
			{
				A[j * n + k] -= A[i * n + j] * A[k * n + i] + A[i * n + k] * A[j * n + i];
			}
			
		//(i+1,i) = (i, i+1) = ||a_i|| others = 0.0
		
		A[(i + 1) * n + i] = x2;
		A[i * n + i + 1] = x2;
		for (j = i + 2; j < n; ++j)
		{
			A[j * n + i] = 0.0;
			A[i * n + j] = 0.0;
		}
	}
}

int change (int n, double *A, double la) 
{
	int i, x;
	double l_ii;
	x = 0;
	
	l_ii = A[0] - la;
	if (l_ii < 0.0) 
	{
		++x;
	}
	
	for (i = 1; i < n; ++i)
	{
		if (fabs(l_ii) < 1e-100)
			l_ii = 1e-10;

		l_ii = A[i * n + i] - la - (A[i * n + i - 1] * A[(i - 1) * n + i])/ l_ii;

		if (l_ii < 0)
			++x;
	}
	return x;
}

void EV_k (int n, double *A, int k, double EPS, double *value, int *count_iter)
{
	int i, j;
	double l, r, m;
	
	i = 0;
	r = Norm(n, A);
	if (fabs(r) < 1e-20)
		r += 1e-10;
	l = -r;
	
	To3D(n, A);
	/*for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			printf ("%lf ", A[i*n + j]);
		}
		printf("\n");
	} */
	
	while (r - l > EPS) 
	{
		m = 0.5 *(l + r);
		if (change(n, A, m) < k)
			l = m;
		else
			r = m;
		++i;
	}
	
	*value = 0.5 *(l + r);
	*count_iter = i;
}

double Norm(int n, double *A)
{
	int i, j;
	double tmp, x;

	x = 0.0;
	for (i = 0; i < n; ++i)
	{
		tmp = 0.0;
		for (j = 0; j < n; ++j)
			tmp += fabs(A[i * n + j]);

		if (x < tmp)
			x = tmp;
	}

	return x;
}
