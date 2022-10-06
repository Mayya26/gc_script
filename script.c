#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1

//структура содержит массив координат и зарядов. В отдельном массиве хранятся символы элементов
//а так же количество точек км/мм
typedef struct MATRIX
{
  double **input;
  int gc, mm;
  char **name;
  char set[20][100];
} matrix_t;

matrix_t create(int rows, int columns);
void remove_matrix(matrix_t *A, int sum);
void print_matrix(matrix_t data);
void setting(matrix_t *A);
void write_conf_file(matrix_t A);
void write_QM(matrix_t A);
void write_MM(matrix_t A);


int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  FILE *input;
  input = fopen(argv[1], "r");
  int n, m;
  fscanf(input, "%d %d", &n, &m);
  matrix_t data = create(n, m);
  for (size_t i = 0; i < n; ++i) {
    fscanf(input, "%lf %lf %lf %c%c", &data.input[i][0], &data.input[i][1], &data.input[i][2], &data.name[i][0], &data.name[i][1]);
    if (data.name[i][1] == '\n') data.name[i][1] = 0;
  }
  for (size_t i = n; i < m+n; ++i) {
    fscanf(input, "%lf %lf %lf %lf", &data.input[i][0], &data.input[i][1], &data.input[i][2], &data.input[i][3]);
  }
  // print_matrix(data);
  // write_conf_file(data);
  fclose(input);
  write_MM(data);
  write_QM(data);
  remove_matrix(&data, n+m);
  return 0;
}

//просто выводит в терминал то что есть в структуре (кроме настроек)
void print_matrix(matrix_t data) {
    for (size_t i = 0; i < data.gc+data.mm; ++i) {
    if (i < data.gc) printf("%f %f %f %c%c\n", data.input[i][0], data.input[i][1], data.input[i][2], data.name[i][0], data.name[i][1]);
    else printf("%f %f %f %f\n", data.input[i][0], data.input[i][1], data.input[i][2], data.input[i][3]);
  }
}

//выделяем функцию под структуру + вызов функции для настроек
matrix_t create(int rows, int columns) {
  matrix_t res;
  if (rows > 0 && columns > 0) {
    res.input = (double **)calloc(rows + columns, sizeof(double *));
    for (size_t i = 0; i < rows + columns; ++i) {
      res.input[i] = (double *)calloc(4, sizeof(double));
    }
    res.name = (char **)calloc(rows, sizeof(char*));
    for (size_t i = 0; i < rows; ++i) {
      res.name[i] = (char *)calloc(2, sizeof(char));
    }
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
      for (size_t i = 0; i < A->gc; ++i) free(A->name[i]);
      free(A->name);
  }
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

//суммирует все заряды
double sum_charges(matrix_t A) {
  double result = 0;
  for (size_t i = A.gc; i < A.gc + A.mm; ++i) {
    result += A.input[i][3];
  }
  return result;
}

//запись в файл настроек. (пока не понятно а нужно ли оно вообще)
void write_conf_file(matrix_t A) {
  FILE *out;
  out = fopen("outQMFile.txt", "w");
  size_t k;
  for (size_t i = 0; i < 20; ++i) {
    k = 0;
     while (A.set[i][k]) {
      fprintf(out, "%c", A.set[i][k]);
      k++;
    }
  }
  fclose(out);
}

//тут это запись в отдельный файл с КМ и символами элементов (формат как в qmmm.xyz)
void write_QM(matrix_t A) {
  FILE *out;
  fopen("qm_out.xyz", "w");
  fprintf(out, "%d\n\n", A.gc);
  for (size_t i = 0; i < A.gc; ++i) {
    if (A.name[i][1] != 0) fprintf(out, "  %c%c ", A.name[i][0], A.name[i][1]);
    else fprintf(out, "  %c ", A.name[i][0]);
    fprintf(out, "%.6lf %.6lf %.6lf\n", A.input[i][0], A.input[i][1], A.input[i][2]);
  }
  fclose(out);
}

//тут запись в отдельный файл с ММ и зарядами. формат как у файла point_charges)
void write_MM(matrix_t A) {
  FILE *out;
  fopen("mm_out.txt", "w");
  fprintf(out, "%d\n\n", A.mm);
  for (size_t i = A.gc; i < A.mm; ++i) {
    fprintf(out, "%.6lf %.6lf %.6lf %.6lf\n", A.input[i][3], A.input[i][0], A.input[i][1], A.input[i][2]);
  }
  fclose(out);
}

//