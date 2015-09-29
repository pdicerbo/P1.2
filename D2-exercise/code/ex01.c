/*
 * Copyright (C) 2001-2013 The Abdus Salam, International Centre of Theoretical Physics (ICTP)
 *
 * This file is distributed under the terms of the GNU General Public License. 
 * See http://www.gnu.org/copyleft/gpl.txt 
 *
 * The code was developed for the hands-on session of M1.2 - HPC Master SISSA/ICTP
 * The example is extracted from: 
 * "Introduction to High Performance Computing for Scientists and Engineers", 
 * by Georg Hager and Gerhard Wellein
 *
 * Author: Ivan Girotto
 * Last update: Sept-2015
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define LOOP_SIZE 1000
#define VECTOR_SIZE 1000000

double seconds(){

/* Return the second elapsed since Epoch (00:00:00 UTC, January 1, 1970) */
  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
  return sec;
}

void dummy(double * temp){
  (* temp) = 4.0;
}


//The funcion compute(...) return the elapsed time needed to complete the vetor upgrade
//Number of operation are stored into the variable n_op
double compute( double * vector, double scalar, int offset, size_t * n_op ){

  int count, i;
  double tstart, tend;

  tstart = seconds();

  for( count = 0; count < LOOP_SIZE; count++ ){
    for( i = 1; i < VECTOR_SIZE - 1; i++){
      vector[i] = scalar * vector[i + offset];
    }
    // leave this dummy check to avoid compiler optimization between the two loops
    if( vector[VECTOR_SIZE/2] < 0 ) dummy(&vector[VECTOR_SIZE/2]);
  }
  
  // compute the number of operations performed
  /* each cycle step perform one sum and one multiplication;  */
  *n_op = LOOP_SIZE * ( VECTOR_SIZE - 2 ) * 2;
  tend = seconds();
  /*return the elapsed time */
  return (tend - tstart);

}

int main(int argc, char * argv[]){

  // allocate and initialize the vector
  double* vector;
  size_t n_operations;
  int offset;
  int j;
  double t_used, scal = 0.5;
  
  if(argc < 2){
    printf("\n\tUsage:\n\t./ex01.x offset\n\toffset must be int type\n");
    exit(0);
  }

  offset = atoi(argv[1]);
  
  vector = (double *)malloc(VECTOR_SIZE * sizeof(double));

  for(j = 0; j < VECTOR_SIZE; j++)
    vector[j] = (double) (j / VECTOR_SIZE);

  t_used = compute(vector, scal, offset, &n_operations);
  
  printf("\n\ttotal number operations: %ld\n", n_operations);
  printf("\ttotal time used: %lg\n", t_used);

  return 0;

}
