#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/* SIZE is the number of 'doubles' in the arrays inputA, inputB and output */
#define SIZE 60000000

int main (int argc, char* argv[]) {

  struct timespec start, end;

	double times;
  double a2,b2;
  int i;

  /* Setup memory */
  int arraySize = SIZE;
  size_t bufferSize = arraySize * sizeof(double);

  double* inputA = (double*) malloc (bufferSize); 
  double* inputB = (double*) malloc (bufferSize);
  double* output = (double*) malloc (bufferSize);    

  /* Initialize data */
  for (i = 0; i < arraySize; i++) {
    inputA[i] = i+1;
    inputB[i] = i+1;
  }

  /* Start time measurements */
  clock_gettime(CLOCK_MONOTONIC, &start);

  /* Computation and memory accesses */
  #pragma omp parallel for private(a2, b2)
  for (i = 0; i < arraySize; i++) {
    a2 = cbrt( inputA[i]*inputA[i] / M_PI ) ;
    b2 = cbrt( inputB[i]*inputB[i] * M_PI_2 ) ;
    output[i] = log(sqrt(a2+b2));
  }

  /* End time measurements */
  clock_gettime(CLOCK_MONOTONIC, &end);

  /* Accumulate results */
  double res=0;
  for (i = 0; i < arraySize; i++) {
    res = res + output[i] ;
  }

  /* Cleaning */
  free(inputA);
  free(inputB);
  free(output);

	/* Compute elapsed milliseconds */
  times = (((double)(end.tv_sec  - start.tv_sec)*1000) +
      ((double)(end.tv_nsec - start.tv_nsec)/1000000));

  printf("Time to finish %6.0f ms [check = %e]\n", times, res);
}
