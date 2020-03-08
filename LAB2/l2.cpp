#include <iostream>
#include <iomanip>
#include <cmath>

#include "cmath.h"

bool multiplyMatrices(const int, const int, const double *, 
      const int, const int, const double *, 
      const int, const int, double *); //return 0 - all is OK, 1 - smth was wrong
double computeNorm(const int, const int, double *);
void printMatrix(const int, const int, const double *);


int main()
{
  const int nPValues = 11;
  double arrPValues[nPValues];
  for (int i = 0; i < nPValues; ++i) {
    arrPValues[i] = pow(10, -i);
  }

  const int mSize = 8 * 8;
  //first value depends by parametr P
  double arrMatrix[mSize] = 
      { /*p + */ 6, 2, 6, 8, -2, 1, 8, -5,
        6, -22, -2, -1, 0, 5, -6, 4,
        -2, -3, -16, 0, 0, -4, 2, -5,
        1, 1, 4, 9, 1, 0, 0, -6,
        0, 2, 0, 2, -3, -5, 7, 5,
        6, -2, -4, 2, -8, -12, 3, -3,
        -6, -6, 0, -8, 0, 5, -15, 0,
        0, 7, 6, 0, -5, -8, -5, -3 };
  
  const int firstElementConstValue = 6;

  //create a new matrix because DECOMP will change the main one 
  double arrMatrixCopy[mSize];
  for (int i = 0; i < mSize; ++i) {
    arrMatrixCopy[i] = arrMatrix[i];
  }

  const int n = sqrt(mSize); //count of rows and cols

  int pivot[n];
  int flag = 0;
  double cond = 0.0;

  double arrColumnEMatrix[n];
  double arrInvertMatrix[mSize] = { 0 };

  //the main loop---------
  for (int i = 0; i < nPValues; ++i) {
    arrMatrix[0] = firstElementConstValue + arrPValues[i];

    //---DECOMP---
    decomp(n, n, arrMatrix, &cond, pivot, &flag);
    std::cout << "cond = " << cond << '\n';

    //-----SOLVE
    int tmp = 0;
    for (int j = 0; j < n; ++j) {
      //get current e_k
      for (int k = 0; k < n; ++k) {
        if (k != j) {
          arrColumnEMatrix[k] = 0;
        }
        else {
          arrColumnEMatrix[k] = 1;
        } 
      }
    
      solve(n, n, arrMatrix, arrColumnEMatrix, pivot);

      //fill invert matrix by columns
      for (int k = 0; k < n; ++k) {
        int a = tmp + n * k;
        arrInvertMatrix[a] = arrColumnEMatrix[k];
      }

      ++tmp;
    }
    //SOLVE------

    //print inverse matrix
    printMatrix(n, n, arrInvertMatrix);
    std::cout << "---------\n";

    //update spoiled matrix
    for (int j = 0; j < mSize; ++j) {
      arrMatrix[j] = arrMatrixCopy[j];
    }
    arrMatrix[0] = firstElementConstValue + arrPValues[i];

    //compute A * A^{-1} = E
    double arrEMatrix[mSize] = { 0 };
    if (!multiplyMatrices(n, n, arrMatrix, n, n, arrInvertMatrix, n, n, arrEMatrix)) {
      printMatrix(n, n, arrEMatrix);
    }
    std::cout << "+++++++++\n";

    //compute R = E - A * A^{-1}
    int index = 0;
    for (int j = 0; j < n; ++j) {
      for (int k = 0; k < n; ++k) {
        index = j * n + k;
        if (j != k) {
          arrEMatrix[index] = 0 - arrEMatrix[index];
        }
        else {
          arrEMatrix[index] = 1 - arrEMatrix[index];
        }
      }
    }
    printMatrix(n, n, arrEMatrix);
    std::cout << "!!!!!!!!!\n";

    std::cout << "Norm = " << computeNorm(n, n, arrEMatrix) << "\n\n";
  }
  //the main loop---------

  return 0;
}

double computeNorm(const int rows, const int cols, double *matrix) //max line
{
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Size of matrix must be positive!");

    return 0.0;
  }

  double max = 0.0;
  double sum = 0.0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      sum += fabs(matrix[i * cols + j]);
    }
    if (sum > max) {
      max = sum;
    }
    sum = 0.0;
  }

  return max;
}

void printMatrix(const int rows, const int cols, const double *arrMatrix)
{
  if (rows <= 0 || cols <= 0) {
    throw std::invalid_argument("Size of matrix must be positive!");
  }
  const int mSize = rows * cols;

  for (int i = 0; i < mSize; ++i) {
      if (((i+1) % cols != 0) || (i == 0)) {
        std::cout << std::setw(12) << std::left << arrMatrix[i] << ' ';
      }
      else {
        std::cout << arrMatrix[i] << '\n';
      }
  }
}