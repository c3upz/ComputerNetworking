/*
COEN146 Lab #2 Part 1

Name: Chris Gerbino
Date: 4 / 10 / 20
File description: This file shows how to exploit parallelism in large scale multidimensional matrix multiplication
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <pthread.h>

//I used two so i could see if my matrix multiplication was correct
#define N 2 //1024 
#define M 2 //1024
#define L 2 
//1024

double matrixA[N][M], matrixB[M][L], matrixC[N][L] = {0.0};

pthread_t thread[N];

void printMatrix(int nr, int nc, double matrix[nr][nc]);
void *start_routine(void *ptr); //declaring function


int main() {
	//the code below fills the matrices with random numbers using the rand() func
	srand(time(NULL));

	int i, j, k = 0;

	// filling matrixA with random numbers
	for (i = 0; i < N; i++) {
		for (j = 0; j < M; j++) {
			matrixA[i][j] = rand(); // rand()
		}
	}

	// filling matrixB with random numbers
	for (i = 0; i < M; i++) {
		for (j = 0; j < L; j++) {
			matrixB[i][j] = rand(); // rand()
		}
	}
	
	/* 
		!NOTE: the commented-out code below was given to us in the lab. However, my implementation
			removes this triple nested four loop and instead I recreate it in the start_routine 
			function. I do this by using the the two nested forloops from below within start_routine,
			and passing in the value of 'i' from the outer most forloop into the start_routine fucntion.
			My implementation has the same affect as the code below.
	*/

   	//for(i = 0; i < N; i++)
    //	for(j = 0; j < L; j++)
	//		for ( k = 0; k < M; k++)
    //			matrixC[i][j] += matrixA[i][k] * matrixB[k][j];

	//This is where the threads are created
	for(i = 0; i < N; i++){ //using N here bc the # of rows in matrixA = # of threads needed
		int *arg = malloc(sizeof(int)); //allocating memory... *arg will be NULL if allocation fails
		*arg = i; //arg will contain the 'thread number' 
		pthread_create(&thread[i], NULL, start_routine, arg);
	}
	
	printf("\n\nMatrixA:\n");
	printMatrix(N, M, matrixA);
	printf("\n\nMatrixB:\n");
	printMatrix(M, L, matrixB);
	printf("\n\nMatrixC:\n");
	printMatrix(N, L, matrixC); 
	
  return 0;
}

void *start_routine(void *arg){
	/*casting void variable as int... this should contain 
		the 'i' value from the forloop that is in charge of creating threads */
	int i = *((int*) arg); 

	int j = 0;
	int k = 0;

	for(j = 0; j < L; j++){ //loop traverses rows
		//double temp = 0;
		for(k = 0; k < M; k++){ // M = # of columns in matrixC
			//multiplying matrices and adding them to the return matrix @ desired index
			matrixC[i][j] += matrixA[i][k] * matrixB[k][j]; //note all matrice values start at 0
		}
	}
	free(arg); // freeing the allocated memory from varibale 'args'
}

void printMatrix(int nr, int nc, double matrix[nr][nc]) {
	for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j ++) {
            printf("%lf  ", matrix[i][j]);
        }
        printf("\n");
    }
}
