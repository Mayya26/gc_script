
#include "s21_matrix.h"


// рандомное заполнение матрицы (для тестов)
void ran(matrix_t *A) {
  if (A->rows > 0 && A->columns > 0) {
    unsigned int r[5];
    srand(time(NULL));
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = rand() % 10;
      }
    }
    A->matrix_type = CORRECT_MATRIX;
  }
}

// заполнение матрицы (для тестов)
void matrix(matrix_t *A, int n) {
  srand(time(NULL));
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      int k = rand() % 10;
      double l = n + rand() % 10;
      A->matrix[i][j] = 1 + i + j * i + k / (l * n);
    }
  }
  A->matrix_type = CORRECT_MATRIX;
}

matrix_t s21_create_matrix(int rows, int columns) {
  matrix_t res;
  if (rows > 0 && columns > 0) {
    res.matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
      res.matrix[i] = (double *)malloc(columns * sizeof(double *));
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        res.matrix[i][j] = 0;
      }
    }
    res.matrix_type = ZERO_MATRIX;
    res.rows = rows;
    res.columns = columns;
  } else {
    res.matrix_type = INCORRECT_MATRIX;
    res.matrix = NULL;
  }
  return res;
}

// посмотреть на матрицу. Потом убрать мб надо! (или хотя бы закоментить)
// void look_matrix(matrix_t res) {
//   if (res.matrix_type != INCORRECT_MATRIX) {
//     for (int i = 0; i < res.rows; i++) {
//       for (int j = 0; j < res.columns; j++) {
//         printf("%lf  ", res.matrix[i][j]);
//       }
//       printf("\n");
//     }
//     printf("rows %d\n", res.rows);
//     printf("columns %d\n", res.columns);
//     printf("type %d\n", res.matrix_type);
//   } else {
//     printf("type %d", res.matrix_type);
//   }
// }

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL) {
    A->matrix_type = INCORRECT_MATRIX;

    if (A->matrix != NULL) {
      int rows = A->rows;

      for (int i = 0; i < rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);

      A->matrix = NULL;
    }

    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int res = SUCCESS;
  if (A->matrix_type != INCORRECT_MATRIX &&
      A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows == B->rows && A->columns == B->columns) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPS) {
            res = FAILURE;
          }
        }
      }
    } else {
      res = FAILURE;
    }
  } else {
    res = FAILURE;
  }
  return res;
}

matrix_t s21_sum_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX &&
      A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows == B->rows && A->columns == B->columns) {
      res = s21_create_matrix(A->rows, B->columns);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          res.matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
      res.matrix_type = CORRECT_MATRIX;
      set_type(&res);
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_sub_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX &&
      A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows == B->rows && A->columns == B->columns) {
      res = s21_create_matrix(A->rows, B->columns);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          res.matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
      res.matrix_type = CORRECT_MATRIX;
      set_type(&res);
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_mult_number(matrix_t *A, double number) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX) {
    res = s21_create_matrix(A->rows, A->columns);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        res.matrix[i][j] = A->matrix[i][j] * number;
      }
    }
    res.matrix_type = CORRECT_MATRIX;
    set_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_mult_matrix(matrix_t *A, matrix_t *B) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX &&
      A->matrix_type != INCORRECT_MATRIX) {
    if (B->rows == A->columns) {
      res = s21_create_matrix(A->rows, B->columns);
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          for (int k = 0; k < A->columns; k++) {
            res.matrix[i][j] += (A->matrix[i][k] * B->matrix[k][j]);
          }
        }
      }
      res.rows = A->rows;
      res.columns = B->columns;
      res.matrix_type = CORRECT_MATRIX;
      set_type(&res);
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_transpose(matrix_t *A) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX) {
    res = s21_create_matrix(A->columns, A->rows);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        res.matrix[j][i] = A->matrix[i][j];
      }
    }
    res.matrix_type = CORRECT_MATRIX;
    set_type(&res);
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_calc_complements(matrix_t *A) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows == A->columns) {
      res = s21_create_matrix(A->rows, A->columns);
      matrix_t tmp;
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->rows; j++) {
          tmp = s21_minor(A, i, j);
          res.matrix[i][j] = pow(-1, i + j) * s21_determinant(&tmp);
          s21_remove_matrix(&tmp);
        }
      }
      res.matrix_type = CORRECT_MATRIX;
      set_type(&res);
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

double s21_determinant(matrix_t *A) {
  double res = NAN;
  if (A->matrix_type != INCORRECT_MATRIX && A != NULL) {
    if (A->rows == A->columns) {
      res = 0;
      if (A->rows == 2) {
        res += A->matrix[0][0] * A->matrix[1][1] -
               A->matrix[0][1] * A->matrix[1][0];
      } else if (A->rows == 1) {
        res = A->matrix[0][0];
      } else {
        for (int k = 0; k < A->rows; k++) {
          matrix_t temp = s21_minor(A, 0, k);
          res += pow(-1, (double)k) * A->matrix[0][k] * s21_determinant(&temp);
          s21_remove_matrix(&temp);
        }
      }
    }
  }
  return res;
}

matrix_t s21_inverse_matrix(matrix_t *A) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows == A->columns) {
      if (s21_determinant(A) != 0.) {
        matrix_t tmp, tmp_2;
        // res = s21_create_matrix(A->rows, A->rows);
        tmp = s21_calc_complements(A);
        tmp_2 = s21_transpose(&tmp);
        res = s21_mult_number(&tmp_2, 1 / s21_determinant(A));
        res.matrix_type = CORRECT_MATRIX;
        set_type(&res);
        s21_remove_matrix(&tmp);
        s21_remove_matrix(&tmp_2);
      } else {
        res.matrix_type = INCORRECT_MATRIX;
      }
    } else {
      res.matrix_type = INCORRECT_MATRIX;
    }
  } else {
    res.matrix_type = INCORRECT_MATRIX;
  }
  return res;
}

matrix_t s21_minor(matrix_t *A, int i, int j) {
  matrix_t res;
  if (A->matrix_type != INCORRECT_MATRIX) {
    if (A->rows >= 2) {
      res = s21_create_matrix(A->rows - 1, A->columns - 1);
      int f = 0, h = 0;
      for (int i_2 = 0; i_2 < A->rows; i_2++) {
        if (i_2 != i) {
          int h = 0;
          for (int j_2 = 0; j_2 < A->rows; j_2++) {
            if (j_2 != j) {
              res.matrix[f][h] = A->matrix[i_2][j_2];
              h++;
            }
          }
          f++;
        }
      }
      res.matrix_type = CORRECT_MATRIX;
      set_type(&res);
    }
  }
  return res;
}

void set_type(matrix_t *A) {
  int flag = 0;
  if (fabs(A->matrix[0][0]) < EPS) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j]) > EPS) {
          flag = 1;
        }
      }
    }
    if (flag == 0) A->matrix_type = ZERO_MATRIX;
  }
  flag = 0;
  if (fabs(A->matrix[0][0] - 1.) < EPS) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if ((A->matrix[i][j] - 1.) > EPS) {
          flag = 1;
        }
      }
    }
    if (flag == 0) A->matrix_type = IDENTITY_MATRIX;
  }
}
