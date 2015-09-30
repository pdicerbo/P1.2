/* perform matrix transpose in two ways */
/* the way "brute force" and the optimized version  */
/* for contiguous element access in cache */

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
  /* transpose matrix in a "brute force" way */
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
  /* print matrix */
  int i, j;

  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      printf("\t%lg", mat[i * size + j]);
    }
    printf("\n");
  }
}

void optimize_transp(double* mat, double* transp, int size, int block){
  /* general index */
  int i, j;
  /* block index */
  int k, h;
  double* tmp;

  tmp = (double*)malloc(block * block * sizeof(double));

  for(i = 0; i < size; i += block){
    for(j = 0; j < size; j += block){
      for(k = 0; k < block; k++){
	for(h = 0; h < block; h++){
	  tmp[h*block + k] = mat[i*size + j + k*size + h];
	}
      }
      for(k = 0; k < block; k++){
	for(h = 0; h < block; h++){
	  transp[j*size + i + k*size + h] = tmp[k*block + h]; 
	}
      }
    }
  }
  free(tmp);
}

int main(int argc, char* argv[]){

  int block_size, fact, mat_size, i, j;
  double* matrix;
  double* transp;
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
  transp  = (double *)malloc(mat_size * mat_size * sizeof(double));

  /* matrix initialization */
  for(i = 0; i < mat_size; i++)
    for(j = 0; j < mat_size; j++)
      matrix[i * mat_size + j] = (double)(i * mat_size + j); //((double) 2*i - j) / ((double) mat_size);

  /* print_matrix(matrix, mat_size); */

  printf("\n\tROW VERSION\n");  
  t_start = seconds();
  row_transpose(matrix, mat_size);
  t_end = seconds();

  printf("\n\ttime used: %lg s\n", (t_end - t_start));
  print_matrix(matrix, mat_size);

  printf("\n\tOPTIMIZED VERSION\n");

  t_start = seconds();
  optimize_transp(matrix, transp, mat_size, block_size);
  t_end = seconds();
  printf("\n\ttime used: %lg s\n", (t_end - t_start));

  print_matrix(transp, mat_size);

  free(matrix);
  free(transp);

  return 0;
}

