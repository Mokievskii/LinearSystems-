#include <stdio.h>
#include <time.h>
#include <math.h>
#include "matrix.hpp"
#include "solve.hpp"

using namespace std;

int main2 (int argc, char* argv[]);
int main (int argc, char* argv[]) {
   main2(argc, argv);
}

int main2 (int argc, char* argv[]) {
	int n;
	int m;
	int check;
	double *A;
	double *X;
	int *tmp;
	
	clock_t t;
	FILE* fin = NULL;
	
        if (argc > 4 && argc <=2) {
                printf("Should be 2 or 3 arguments\n");
		return -2;
	}
        if (argc == 3) {
		if (!(sscanf(argv[1], "%d", &n))) {
			printf("Incorrect size\n");
			return -1;
		}
                if (!(sscanf(argv[2], "%d", &m))) {
                        printf("Incorrect size\n");
                        return -1;
                }
		if (n <= 0) {
			printf("Incorrect size\n");
			return -1;
		}
	}
        if (argc == 4) {
		if (!(sscanf(argv[1], "%d", &n))) {
			printf("Incorrect size\n");
			return -1;
		}
		if (n <= 0) {
			printf("Incorrect size\n");
			return -1;
		}
		fin = fopen(argv[2], "r");
                if (!fin) {
                printf("File don't exist\n");
                fclose(fin);
                return -3;
                }
                if (!(sscanf(argv[3], "%d", &m))) {
                        printf("Incorrect size\n");
                        return -1;
                }
	}
	
	if (m > n) {
                if (argc == 4)
		{
			fclose(fin);
		}
		printf("Incorrect size of output\n");
		return -1;
	}
	
	A = new double [n*n];
	X = new double [n*n];
	tmp = new int [n];
	if (!(A && X && tmp)) {
		printf("No memory, enter matrix with less dimensions\n");
        
        if (argc == 4)
            fclose(fin);
        
        delete []A;
        delete []X;
        delete []tmp;
        return -4;
	}
	check = enter_data(A, n, fin);
	if (check != 0) {
		printf("Data isn't correct\n");

        if (argc == 4)
            fclose(fin);
        
        delete []A;
        delete []X;
        delete []tmp;
        return -5;
	}
	
	printf("\n");
    t = clock();
    check = solveSystem(n, A, X, tmp);
    t = clock() - t;
    
    if (check != -1)
    {
        printf("Result:\n");
        print_res(X, n, m);
        fclose(fin);
        fin = fopen(argv[2], "r");
        enter_data(A, n, fin);
        printf("Error norm: %e\n", error_norm(A, X, n));
        
        printf("Solving time =  %lu seconds\n", t/CLOCKS_PER_SEC);
    }
    else
    {
        printf("Error while solving \n");
        
        if (argc == 4)
            fclose(fin);
        
        delete []A;
        delete []X;
        delete []tmp;
        return -1;
    }
    if (argc == 4)
        fclose(fin);
    delete []A;
    delete []X;
    delete []tmp;
	return 0;
}
