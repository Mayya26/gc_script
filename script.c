#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double **create_matrix(int rows, int columns);

int main() {
  FILE *input;
  input = fopen("input.txt", "r");
  double **matrix_inp;
  int n, m;
  char ch;
  fscanf(input, "%d %d", &n, &m);
  matrix_inp = create_matrix(n + m, 4);
  for (size_t i = 0; i < n; ++i) {
    fscanf(input, "%lf %lf %lf", &matrix_inp[i][0], &matrix_inp[i][1], &matrix_inp[i][2]);
  }
  for (size_t i = n; i < m+n; ++i) {
    fscanf(input, "%lf %lf %lf %lf", &matrix_inp[i][0], &matrix_inp[i][1], &matrix_inp[i][2], &matrix_inp[i][3]);
  }
  for (size_t i = 0; i < n+m; ++i) {
    printf("%f %f %f %f\n", matrix_inp[i][0], matrix_inp[i][1], matrix_inp[i][2], matrix_inp[i][3]);
  }
  fclose(input);
  return 0;
}


double **create_matrix(int rows, int columns) {
  double **res;
  if (rows > 0 && columns > 0) {
    res = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
      res[i] = (double *)calloc(columns, sizeof(double *));
    }
  } 
  return res;
}
