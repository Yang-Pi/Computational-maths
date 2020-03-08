#include <iostream>

bool multiplyMatrices(const int rows1, const int cols1, const double *arrMatrix1,
      const int rows2, const int cols2, const double *arrMatrix2,
      const int rowsRes, const int colsRes, double *arrMatrixRes)
{
  if (rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <= 0 || rowsRes <= 0 || colsRes <= 0) {
    throw std::invalid_argument("Size of matrix must be positive!");

    return 1;
  }
  if (cols1 != rows2) {
    throw std::invalid_argument("Matrices can't be multiplied!");
    
    return 1;
  }
  if (rowsRes != rows1 || colsRes != cols2) {
    throw std::invalid_argument("Result matrix has invalid size!");

    return 1;
  }

  int index = 0;
  for (int i = 0; i < rowsRes; ++i) {
    for (int j = 0; j < colsRes; ++j) {
      index = i * colsRes + j;
      arrMatrixRes[index] = 0;
      for (int k = 0; k < cols1; ++k) {
        arrMatrixRes[index] += arrMatrix1[i * cols1 + k] * arrMatrix2[k * cols2 + j];
      }
    }
  }

  return 0;
}