#include <iostream>
#include <iomanip>
#include <cmath>

#include "cmath.h"

int fun(int, double, double *, double *);

int main()
{
  const double startValue1 = -3.0;
  const double startValue2 = 1.0;

  const int N = 2;
  int fail = 0;

  rkfinit(N, &fail); //RKFINIT()
  if (fail) {
    return 1;
  }

  //---other arguments for RKF45()

  double arrValues[N] = {startValue1, startValue2};
  double arrDxDt[N] = {0.0};
  
  double t = 0.0;
  double tOut = 0.0;

  double relerr = 1.0e-4;
  double abserr = 1.0e-4;

  double h = 0.0;
  int nfe = 0;
  const int maxnfe = 5000; //take value from example
  int flag = 1;
  //---

  const double HPRINT = 5.0e-2;

  //compute number of iterations for the loop
  const double t1 = 0.0;
  const double t2 = 1.0;
  int nIter = static_cast<int>(abs((t2-t1)) / HPRINT);

//-----TASK1_____RKF45()
  for (int i = 0; i < nIter; ++i) {
    tOut += HPRINT;
    rkf45(fun, N, arrValues, arrDxDt, &t, tOut, &relerr, abserr, &h, &nfe, maxnfe, &flag);

    std::cout << "t = " << t << "  " << arrValues[0] << "  " << arrValues[1] << '\n';
  }
//TASK1-----

std::cout << "------------------------------------------------\n";

//-----TASK2_____Adams (2) method (Linear multistep method)

  //x_{n+1} = x_n + 0.5 * h * (3 * f_n - f_{n-1}) -- Adams (2) method (Linear multistep method)

  double fN[N] = {0.0}; //f_n
  double fNm1[N] = {0.0}; //f_{n-1}

  //---compute all starting values
  arrValues[0] = startValue1;
  arrValues[1] = startValue2;

  fun(N, t1, arrValues, fN); //compute f_n

  const double HINT = 2.5e-3;

  //we know x_1(0) and x_2(0) but also need x_1(0 - HINT) and x_2(0 - HINT) as starting valuses for Adams method
  tOut = t1 - HINT; //it's less than 0
  t = t1; //it's 0
  //tOut <-- t (not t --> tOut)
  rkf45(fun, N, arrValues, arrDxDt, &t, tOut, &relerr, abserr, &h, &nfe, maxnfe, &flag);

  fun(N, t, arrValues, fNm1); //compute f_{n-1}
  //---

  nIter = static_cast<int>(abs((t2-t1)) / HINT);
  t = t1;

  for (int i = 0; i < nIter; ++i) {
    for (int j = 0; j < N; ++j) {
      arrValues[j] = arrValues[j] + 0.5 * HINT * (3 * fN[j] - fNm1[j]); //formula for the method
      fNm1[j] = fN[j];
    }
    t += HINT;
    fun(N, t, arrValues, fN);

    std::cout << std::setw(7) << std::left << t << std::setw(10) << std::left << arrValues[0] << std::setw(10) << std::left << arrValues[1] << '\n';
  }
//TASK2-----

  rkfend(); //RKFEND()

  return 0;
}

int fun(int n, double t, double *arrValues, double *arrDxDt)
{
  for (int i = 0; i < n; ++i) {
    switch (i) 
    {
      //X_1 = arrValues[0], X_2 = arrValues[1]
      case 0 :
        arrDxDt[i] = (-73) * arrValues[0] - 210 * arrValues[1] + log(1 + pow(t, 2));
        break;

      case 1 :
        arrDxDt[i] = arrValues[0] + exp(-t) + pow(t, 2) + 1;
        break;
      
      default:
        std::cerr << "The number of ODE's is bigger than known count of equations!";
        return 1;
    }
  }

  //std::cout << t << "   " << arrValues[0] << "   " << arrValues[1] << " ... " << arrDxDt[0] << "   " << arrDxDt[1] << '\n';

  return 0;
}