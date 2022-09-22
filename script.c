#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//исправить потом, что он умеет считывать только элекменты из одного символа

typedef struct MATRIX
{
  double **input;
  int gc, mm;
  char *name;
} matrix_t;

matrix_t create(int rows, int columns);
void remove_matrix(matrix_t *A, int sum);
void print_matrix(matrix_t data);



int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  FILE *input;
  input = fopen(argv[1], "r");
  int n, m;
  fscanf(input, "%d %d", &n, &m);
  matrix_t data = create(n, m);
  for (size_t i = 0; i < n; ++i) {
    fscanf(input, "%lf %lf %lf %c", &data.input[i][0], &data.input[i][1], &data.input[i][2], &data.name[i]);
  }
  for (size_t i = n; i < m+n; ++i) {
    fscanf(input, "%lf %lf %lf %lf", &data.input[i][0], &data.input[i][1], &data.input[i][2], &data.input[i][3]);
  }
  // print_matrix(data);
  fclose(input);
  remove_matrix(&data, n+m);
  return 0;
}

void print_matrix(matrix_t data) {
    for (size_t i = 0; i < data.gc+data.mm; ++i) {
    if (i < data.gc) printf("%f %f %f %c\n", data.input[i][0], data.input[i][1], data.input[i][2], data.name[i]);
    else printf("%f %f %f %f\n", data.input[i][0], data.input[i][1], data.input[i][2], data.input[i][3]);
  }
}

matrix_t create(int rows, int columns) {
  matrix_t res;
  if (rows > 0 && columns > 0) {
    res.input = (double **)calloc(rows + columns, sizeof(double *));
    for (int i = 0; i < rows + columns; i++) {
      res.input[i] = (double *)calloc(4, sizeof(double));
    }
    res.name = (char *)calloc(rows, sizeof(char));
    res.gc = rows;
    res.mm = columns;
  } 
  return res;
}


void remove_matrix(matrix_t *A, int sum) {
  if (A != NULL) {
      for (int i = 0; i < sum; i++) {
        free(A->input[i]);
      }
      free(A->input);
  }
  free(A->name);
  A->gc = 0;
  A->mm = 0;
}

