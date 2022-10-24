#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
// #include "matrix/s21_matrix.h"

#define TRUE 1
#define Const 0.52917721067
#define a0 0.52917721067
#define conversionHB  627.509469/a0

//структура содержит массив координат и зарядов. В отдельном массиве хранятся символы элементов
//а так же количество точек км/мм
typedef struct DATA
{
  double **input;
  int gc, mm;
  char **name;
  char set[20][100];
} data_t;


data_t create(int rows, int columns);
void remove_matrix(data_t *A, int sum);
void print_matrix(data_t data);
void setting(data_t *A);
void write_conf_file(data_t A);
void write_QM(data_t A);
void write_MM(data_t A);
double **dist_py(data_t A);
int factorial(int n);
int **triu_indices(int len);
double computePointChargeSelfEnergy(data_t A);
double *computePointChargeSelfGradient(data_t A);


int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  FILE *input;
  input = fopen(argv[1], "r");
  int n, m;
  fscanf(input, "%d %d", &n, &m);
  data_t data = create(n, m);
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
  dist_py(data);
  computePointChargeSelfEnergy(data);
  computePointChargeSelfGradient(data);
  remove_matrix(&data, n+m);
  return 0;
}

//просто выводит в терминал то что есть в структуре (кроме настроек)
void print_matrix(data_t data) {
    for (size_t i = 0; i < data.gc+data.mm; ++i) {
    if (i < data.gc) printf("%f %f %f %c%c\n", data.input[i][0], data.input[i][1], data.input[i][2], data.name[i][0], data.name[i][1]);
    else printf("%f %f %f %f\n", data.input[i][0], data.input[i][1], data.input[i][2], data.input[i][3]);
  }
}

//выделяем функцию под структуру + вызов функции для настроек
data_t create(int rows, int columns) {
  data_t res;
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
  // setting(&res);
  return res;
}

//очистка структуры
void remove_matrix(data_t *A, int sum) {
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

//скорее всего удалю

//запись из файла настроек в структуру
// void setting(data_t *A) {
//   FILE *input;
//   input = fopen("settings.txt", "r");
//   size_t i = 0, k = 0;
//   while(!feof(input)) {
//     A->set[i][0] = fgetc(input);
//     k = 0;
//     if (A->set[i][0] == 'e') {
//       A->set[i][1] = fgetc(input);
//       if (A->set[i][1] == 'n') {
//         A->set[i][2] = fgetc(input);
//         if (A->set[i][2] == 'd') break;
//       }
//       k = 2;
//     }
//     while (A->set[i][k] != '\n') {
//       k++;
//       A->set[i][k] = fgetc(input);
//     }
//     i++;
//   }

//   fclose(input);
// }

//суммирует все заряды
double sum_charges(data_t A) {
  double result = 0;
  for (size_t i = A.gc; i < A.gc + A.mm; ++i) {
    result += A.input[i][3];
  }
  return result;
}

//запись в файл настроек. (пока не понятно а нужно ли оно вообще)
//вероятно просто будет скрипт, который будет всё запускать. Или баш или через make сделаю
void write_conf_file(data_t A) {
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
void write_QM(data_t A) {
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
void write_MM(data_t A) {
  FILE *out;
  fopen("mm_out.xyz", "w");
  fprintf(out, "%d\n\n", A.mm);
  for (size_t i = A.gc; i < A.mm; ++i) {
    fprintf(out, "%.6lf %.6lf %.6lf %.6lf\n", A.input[i][3], A.input[i][0], A.input[i][1], A.input[i][2]);
  }
  fclose(out);
}

//кусок с расчётами

//расстояние между двумя точками
double distance(data_t A, int i, int j) {
  double result;
  // printf("%lf\n", pow((A.input[j][0] - A.input[i][0]), 2));
  result = pow((A.input[i][0] - A.input[j][0]), 2);
  result += pow((A.input[i][1] - A.input[j][1]), 2);
  result += pow((A.input[i][2] - A.input[j][2]), 2);
  result = sqrt(result) / Const;
  return result;
}

//тут матричные вычисления. Будет матрица mm*mm. Будет больно оптимизировать
double **dist_py(data_t A) {
  double **result;
  result = (double **)malloc(A.mm * sizeof(double *));
  for (size_t i = 0; i < A.mm; i++) {
    result[i] = (double *)malloc(A.mm * sizeof(double ));
  }
  for (size_t i = 0; i < A.mm; ++i) {
    result[i][i] = 0;
    for (size_t j = 0; j < A.mm; ++j) {
      if (!(result[i][j] > 0)) result[i][j] = result[j][i] = distance(A, i + A.gc, A.gc + j);
    }
  }
  return result;
}

//индексы верхней треугольной матрицы
int **triu_indices(int len) {
  int **result;
  int f = factorial(len);
  result = (int **)malloc(2 * sizeof(int *));
  result[0] = (int *)malloc(f * sizeof(int ));
  result[1] = (int *)malloc(f * sizeof(int ));
  int k = 0;
  for (size_t i = 0; i < len - 1; ++i) {
    for (size_t t = i + 1; t < len; ++t) {
      result[0][k] = i;
      result[1][k] = t;
      k++;
    }
  }
  return result;
}


int factorial(int n) {
  int result = 0;
  for (size_t i = 1; i <= n; ++i) {
    result += i;
  }
  return result;
}


//выдаёт бесконечность на 11.10 в 4:20 утра. В какой-то момент времени оно становится равно 0. 
double computePointChargeSelfEnergy(data_t A) {
  double energe = 0;
  int length = A.mm;
  int f = factorial(A.mm - 1);
  double **result;
  result = dist_py(A);
  int **index;
  index = triu_indices(length); 
  for (size_t i = 0; i < f; ++i) {
    energe += A.input[index[0][i] + A.gc][3] * A.input[index[1][i] + A.gc][3] / result[index[0][i]][index[1][i]];
  }
  // printf("%lf\n", energe);
  for (int i = 0; i < A.mm; i++) {
    free(result[i]);
  }
  free(result);
  free(index[0]);
  free(index[1]);
  free(index);
  return energe;
}

//pcgrad[i]+=((coords[i,:]-coords[j,:])  *pc[i,3]*pc[j,3]   /r3[i,j])*0.52917721067*0.52917721067
double *computePointChargeSelfGradient(data_t A) {
  double *pcgrad;
  pcgrad = (double *)calloc(A.mm, sizeof(double));
  double **r;
  r = dist_py(A);
  size_t i = 0;
  #pragma omp parallel
  #pragma omp for
  for (i = 0; i < A.mm; ++i) {
    for (size_t j = 0; j < A.mm; ++j) {
      if (i != j) {
      pcgrad[i] += ((A.input[i + A.gc][0] - A.input[j + A.gc][0]) * A.input[i + A.gc][3]*A.input[j + A.gc][3]/r[i][j])*Const * Const;
      pcgrad[i] += ((A.input[i + A.gc][1] - A.input[j + A.gc][1]) * A.input[i + A.gc][3]*A.input[j + A.gc][3]/r[i][j])*Const * Const;
      pcgrad[i] += ((A.input[i + A.gc][2] - A.input[j + A.gc][2]) * A.input[i + A.gc][3]*A.input[j + A.gc][3]/r[i][j])*Const * Const;
      }
    }
    pcgrad[i] *= -conversionHB;
    // printf("%lf\n", pcgrad[i]);
  }
  return pcgrad;
}