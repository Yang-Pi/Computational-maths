#include <iostream>
#include <iomanip>
#include <cmath>

#include "cmath.h"

const int X_LIMIT_1 = 1;
const int X_LIMIT_2 = 4;
const double H = 0.375;

const double A = 0;
const double B = 20;

double fun(double);
double checkFun(double);

double lagrange(int, double *, double *, int, double);

void printResults(const int, double *, double *, const int, double *, double *, double *);

int main()
{
  std::cout << "FUNCTION = " << "1 / ( exp(z) * (z + x) ); \n\n";

//INPUT parameters
  const double epsabs = 0.0;
  const double epsrel = 1.0e-10;
//OUTPUT parameters
  double result = 0.0;
  double errest = 0.0;
  int nofun = 0;
  double posn = 0.0;
  int flagQuanc = 0;

  const int n = 1 + static_cast<int>(abs(X_LIMIT_1 - X_LIMIT_2) / H);

  double arrQuancRes[n] = { 0 };

//---------QUANC8---------
  for (int i = 0; i < n; ++i) {
    quanc8(fun, A, B, epsabs, epsrel, &result, &errest, &nofun, &posn, &flagQuanc);
    arrQuancRes[i] = result;
  }

  std::cout << "FLAG QUANC8 = " << flagQuanc << ";\n";
//---------
  //unite all X values
  double arrXPoints[n];
  for (int i = 0; i < n; ++i) {
    arrXPoints[i] = X_LIMIT_1 + i * H;
  }

  const int k_min = 0;
  const int k_max = 7;
  const int nK = k_max - k_min + 1;

  double arrSplineRes[nK] = { 0 };
  double arrLagrRes[nK] = { 0 };

  //compute X_k values
  double arrXKPoints[nK];
  for (int i = 0; i < nK; ++i) {
    arrXKPoints[i] = 1.1875 + 0.375 * i;
  }

//---------SPLINE---------
  double arrB[nK] = { 0 };
  double arrC[nK] = { 0 }; 
  double arrD[nK] = { 0 };

  int flagSpline = 0;
  int last = 0;

  spline(n, 1, 1, 1, 1, arrXPoints, arrQuancRes, arrB, arrC, arrD, &flagSpline);

  std::cout << "FLAG SPLINE = " << flagSpline << ";\n";

  for (int i = k_min; i < k_max + 1; ++i) {
    arrSplineRes[i] = seval(n, arrXKPoints[i], arrXPoints, arrQuancRes, arrB, arrC, arrD, &last); //SEVAL
  }
//---------

//---------LAGRANGE---------
  arrXKPoints[0] = 1.1875; //it is reliable crutch because something strange have eaten fist value!
  for (int i = k_min; i < k_max + 1; ++i) {
    arrLagrRes[i] = lagrange(n, arrXPoints, arrQuancRes, nK - 1, arrXKPoints[i]);
  }
//---------

  printResults(n, arrXPoints, arrQuancRes, nK, arrXKPoints, arrSplineRes, arrLagrRes);

//analize accuracy using methods (spline and lagrange)---------
  //compute QUANC8 with x_k
  for (int i = 0; i < nK; ++i) {
    quanc8(checkFun, A, B, epsabs, epsrel, &result, &errest, &nofun, &posn, &flagQuanc);
    arrQuancRes[i] = result;
  }

  std::cout << flagQuanc << '\n';

  //simple print
  std::cout << "ANALIZE RESULTS\n";
  for (int i = k_min; i < k_max + 1; ++i) {
    std::cout << arrQuancRes[i] << "   " << arrQuancRes[i] - arrSplineRes[i] << "   " << arrQuancRes[i] - arrLagrRes[i] << '\n';
  }

  return 0;
}

//integral function
double fun(double z)
{
  double x = X_LIMIT_1;
  static int i = -1;

  if (z == A) {
    x = X_LIMIT_1 + (++i * H);
  }
  else {
    x = X_LIMIT_1 + (i * H);
  }

  return { 1 / (exp(z) * (z + x)) };
}

//function for recomputing QUANC8
double checkFun(double z)
{
  double x = 1.1875;
  static int i = -1;
  if (z == A) {
    x = 1.1875 + 0.375 * (++i);
  }
  else {
    x = 1.1875 + 0.375 * i;
  }

  return { 1 / (exp(z) * (z + x)) };
}

//print main results of QUANC8 with X and SPLINE and LAGRANGE with X_k
void printResults(const int n, double *arrXPoints, double *arrQuancRes, 
      const int nK, double *arrXKPoints, double *arrSplineRes, double *arrLagrangeRes)
{
  if (n <= 0 || nK <= 0) {
    throw std::invalid_argument("Tables are empty");
  }

  std::cout << '\n';
  std::cout << "---------QUANC8---------\n";
  for (int i = 0; i < n; ++i) {

    std::cout << "X = " << std::setw(5) << std::left << arrXPoints[i] << "   F(X) = " << arrQuancRes[i] << ";\n";
  }

  std::cout << '\n';
  std::cout << "---------SPLINE & LAGRANGE---------\n";
  for (int i = 0; i < nK; ++i) {
    std::cout << "X_k = " << std::setw(5) << std::left << arrXKPoints[i] << "   SPLINE = " << std::setw(10) << std::left << arrSplineRes[i] 
          << "   LAGR = " << arrLagrangeRes[i] << ";\n";
  }
}
