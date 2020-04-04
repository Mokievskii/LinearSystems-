#include "matrix.hpp"
#include <math.h>

using namespace std;

double func (int i, int j) {
	return (i+1)*(j+2);
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
