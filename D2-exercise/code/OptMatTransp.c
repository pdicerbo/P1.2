#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

double seconds(){

/* Return the second elapsed since Epoch (00:00:00 UTC, January 1, 1970) */
  struct timeval tmp;
  double sec;
  gettimeofday( &tmp, (struct timezone *)0 );
  sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
  return sec;
}

void row_transpose(double* mat, int size){
  int i, j;
  double tmp;

  for(i = 0; i < size; i++){
    for(j = i; j < size; j++){
      tmp = mat[i*size + j];
      mat[i*size + j] = mat[j*size + i];
      mat[j*size + i] = tmp;
    }
  }
}

void print_matrix(double* mat, int size){
  int i, j;

  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      printf("\t%lg", mat[i * size + j]);
    }
    printf("\n");
  }
}

int main(int argc, char* argv[]){

  int block_size, fact, mat_size, i, j;
  double* matrix;
  double t_start, t_end;

  if(argc < 3){
    printf("\n\tUsage:\n\t./OptMatTransp.x block_dim fact\n\tthe matrix dimension will be block_dim * fact\n");
    printf("\tblock_dim and fact must be integer type\n");
    exit(0);
  }

  block_size = atoi(argv[1]);
  fact = atoi(argv[2]);

  mat_size = block_size * fact;

  matrix = (double *)malloc(mat_size * mat_size * sizeof(double));

  /* matrix initialization */
  for(i = 0; i < mat_size; i++)
    for(j = 0; j < mat_size; j++)
      matrix[i * mat_size + j] = ((double) 2*i - j) / ((double) mat_size);

  /* print_matrix(matrix, mat_size); */
  
  t_start = seconds();
  row_transpose(matrix, mat_size);
  t_end = seconds();

  printf("\n\ttime used: %lg s\n", (t_end - t_start));

  /* print_matrix(matrix, mat_size); */

  free(matrix);

  return 0;
}

