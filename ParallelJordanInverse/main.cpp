#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "matrix.hpp"
#include "solve.hpp"

using namespace std;

int main2 (int argc, char* argv[]);
int main (int argc, char* argv[]) {
   main2(argc, argv);
}

typedef struct
{
    int n;
    double *A;
    double *X;
    int *tmp;
    int thread_num;
    int total_threads;
    double* time;
    int* error;
} ARGS;

double get_full_time(void)
{
    struct timeval buf;

    gettimeofday(&buf, 0);

    return buf.tv_sec * 100 + buf.tv_usec/10000;
}


void *solve(void *p_arg)
{
    double start_time, end_time;
    ARGS *args = (ARGS*)p_arg;
    start_time = get_full_time ();
    solveSystem(args->n, args->A, args->X, args->tmp, args->thread_num, args->total_threads, args->error);
    end_time = get_full_time ();
    if (args->thread_num == 0) {
        *(args -> time) = end_time - start_time;
    }

    return NULL;
}

int main2 (int argc, char* argv[]) {
	int n, m, i;
	double time;
	int check;
	double *A;
	double *X;
	int *tmp;
    int total_threads;
    pthread_t *threads;
    ARGS *args;
    int error = 0;
    int INT_MAX = 2147483647;

	FILE* fin = NULL;
	
	if (argc > 5 || argc <4) {
	    printf("Should be 3 or 4 arguments\n ./prog num_input (file_name) num_output num_threads\n");
		return -2;
	}
	if (argc == 4) {
	    if (!(sscanf(argv[1], "%d", &n))) {
	        printf("Incorrect size of input\n");
	        return -1;
	    }
	    if (!(sscanf(argv[2], "%d", &m))) {
	        printf("Incorrect size of output\n");
	        return -1;
	    }
	    if (!(sscanf(argv[3], "%d", &total_threads))) {
	        printf("Incorrect number of threads\n");
	        return -1;
	    }
	    if (n <= 0 || m <= 0 || total_threads <= 0) {
	        printf("Incorrect sizes\n");
	        return -1;
	    }

	}
	if (argc == 5) {
	    if (!(sscanf(argv[1], "%d", &n))) {
	        printf("Incorrect size of input\n");
	        return -1;
	    }
	    fin = fopen(argv[2], "r");
	    if (!fin) {
	        printf("File doesn't exist\n");
	        fclose(fin);
	        return -3;
	    }
	    if (!(sscanf(argv[3], "%d", &m))) {
	        printf("Incorrect size of output\n");
	        return -1;
	    }
        if (!(sscanf(argv[4], "%d", &total_threads))) {
            printf("Incorrect number of threads\n");
            return -1;
        }
        if (n <= 0 || m <= 0 || total_threads <= 0) {
            printf("Incorrect sizes\n");
            return -1;
        }
	}
	
	if (m > n) {
                if (argc == 5)
		{
			fclose(fin);
		}
		printf("Incorrect size of output\n");
		return -1;
	}

    if (n >= sqrt((double)INT_MAX)) {
        printf("Too much dim\n");
        return -1;
    }

	A = new double [n*n];
	X = new double [n*n];
	tmp = new int [n];
    threads = new pthread_t [total_threads];
    args = new ARGS [total_threads];

	if (!(A && X && tmp && threads && args)) {
		printf("No memory, enter matrix with less dimensions\n");
        
        if (argc == 5)
            fclose(fin);
        
        if (A)
            delete []A;
        if (X)
            delete []X;
        if (tmp)
            delete []tmp;
        if (threads)
            delete []threads;
        if (args)
            delete []args;
        return -4;
	}
	check = enter_data(A, n, fin);
	if (check != 0) {
		printf("Data isn't correct\n");

        if (argc == 5)
            fclose(fin);
        
        delete []A;
        delete []X;
        delete []tmp;
        delete []threads;
        delete []args;
        return -5;
	}

    for (i = 0; i < total_threads; i++)
    {
        args[i].n = n;
        args[i].A = A;
        args[i].X = X;
        args[i].tmp = tmp;
        args[i].thread_num = i;
        args[i].total_threads = total_threads;
        args[i].error = &error;
        args[0].time = &time;
    }


    for (i = 0; i < total_threads; i++) {
        if (pthread_create(threads + i, 0, solve, args + i)) {
            printf("Cannot create thread number %d!\n", i);

            if (A)
                delete []A;
            if (X)
                delete []X;
            if (tmp)
                delete []tmp;
            if (threads)
                delete []threads;
            if (args)
                delete []args;

            return -6;
        }
    }
    for (i = 0; i < total_threads; i++) {
        if (pthread_join(threads[i], 0)) {
            printf("Cannot wait thread number %d!\n", i);

            if (A)
                delete []A;
            if (X)
                delete []X;
            if (tmp)
                delete []tmp;
            if (threads)
                delete []threads;
            if (args)
                delete []args;

            return -7;
        }
    }
    //printf("%d\n", *(args[0].error));
    if (*(args[0].error)> 0)
    {
        printf("Error while solving \n");

        if (argc == 4)
            fclose(fin);

        delete []A;
        delete []X;
        delete []tmp;
        delete []threads;
        delete []args;
        return -1;
    }
    printf("Result:\n");
    print_res(X, n, m);
    if (argc == 5) {
        fclose(fin);
        fin = fopen(argv[2], "r");
    }
    enter_data(A, n, fin);
    printf("Error norm: %e\n", error_norm(A, X, n));
    printf("Solving time =  %lf seconds\n", time / 100);

    if (argc == 4)
        fclose(fin);
    delete []A;
    delete []X;
    delete []tmp;
    delete []threads;
    delete []args;

	return 0;
}
