#include <stdio.h>
#include <time.h>
#include <math.h>
#include "matrix.hpp"
#include "solve.hpp"

using namespace std;

int main2 (int argc, char* argv[]);
int main (int argc, char* argv[]) {
	//while (1)
		main2(argc, argv);
}

int main2 (int argc, char* argv[]) {
	int n;
	int k;
	int check;
	int iter;
	double EPS = 1e-10;
	double value;
	double *A;
	
	clock_t t;
	FILE* fin = NULL;
	
        if (argc > 4 || argc <=2) {
                printf("Should be 2 or 3 arguments\n");
		return -2;
	}
        if (argc == 3) {
		if (!(sscanf(argv[1], "%d", &n))) {
			printf("Incorrect size\n");
			return -1;
		}
                if (!(sscanf(argv[2], "%d", &k))) {
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
                if (!(sscanf(argv[3], "%d", &k))) {
                        printf("Incorrect size\n");
                        return -1;
                }
	}
	
	if (k > n || k <= 0) {
                if (argc == 4)
		{
			fclose(fin);
		}
		printf("Incorrect number of EV\n");
		return -1;
	}
	
	A = new double [n*n];


	if (!(A)) {
		printf("No memory, enter matrix with less dimensions\n");
        
        if (argc == 4)
            fclose(fin);
        
        delete []A;
        return -4;
	}
	check = enter_data(A, n, fin);
	if (check != 0) {
		printf("Data isn't correct\n");

        if (argc == 4)
            fclose(fin);
        
        delete []A;
        return -5;
	}
	
	printf("\n");
    t = clock();
    EV_k(n, A, k, EPS, &value, &iter);
    t = clock() - t;
    
		printf("Value:%lf  \n", value);
		printf("\n");
        
        printf("Solving time =  %lu seconds\n", t/CLOCKS_PER_SEC);
        printf("Iterations = %d\n", iter);
    
    if (argc == 4)
        fclose(fin);
    delete []A;
	return 0;
}
