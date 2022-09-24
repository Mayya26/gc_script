#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//исправить потом, что он умеет считывать только элекменты из одного символа


//структура содержит массив координат и зарядов. В отдельном массиве хранятся символы элементов
//а так же количество точек км/мм
typedef struct MATRIX
{
  double **input;
  int gc, mm;
  char *name;
  char set[20][100];
} matrix_t;

matrix_t create(int rows, int columns);
void remove_matrix(matrix_t *A, int sum);
void print_matrix(matrix_t data);
void setting(matrix_t *A);



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
  fclose(input);
  remove_matrix(&data, n+m);
  return 0;
}

//просто выводит в терминал то что есть в структуре (кроме настроек)
void print_matrix(matrix_t data) {
    for (size_t i = 0; i < data.gc+data.mm; ++i) {
    if (i < data.gc) printf("%f %f %f %c\n", data.input[i][0], data.input[i][1], data.input[i][2], data.name[i]);
    else printf("%f %f %f %f\n", data.input[i][0], data.input[i][1], data.input[i][2], data.input[i][3]);
  }
}

//выделяем функцию под структуру + вызов функции для настроек
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
  setting(&res);
  return res;
}

//очистка структуры
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


//запись из файла настроек в структуру
void setting(matrix_t *A) {
  FILE *input;
  input = fopen("settings.txt", "r");
  size_t i = 0, k = 0;
  while(!feof(input)) {
    A->set[i][0] = fgetc(input);
    k = 0;
    if (A->set[i][0] == 'e') {
      A->set[i][1] = fgetc(input);
      if (A->set[i][1] == 'n') {
        A->set[i][2] = fgetc(input);
        if (A->set[i][2] == 'd') break;
      }
      k = 2;
    }
    while (A->set[i][k] != '\n') {
      k++;
      A->set[i][k] = fgetc(input);
    }
    i++;
  }
  fclose(input);
}