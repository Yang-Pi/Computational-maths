#define _USE_MATH_DEFINES

#include <iostream>
#include <stdexcept>
#include "cmath"

double LAGR(int , double arrPoints[], double arrFunctions[], int m, double x);

int main ()
{
  const int n = 4;
  double arrPoints[n] = {2.3, 2.4, 2.5, 2.6};
  double arrFunctions[n] = {};
  for (int i = 0; i < n; i++) {
    arrFunctions[i] = log(arrPoints[i] + 1);
  }
  double x = 2.338105;
  try {
    double result = LAGR(n, arrPoints, arrFunctions, 4, x);
    std::cout << result << '\n';
  } catch(std::invalid_argument & err) {
    std::cerr << err.what();
  }
}

double LAGR(int n, double arrPoints[], double arrFunctions[], int m, double x)
{
  if (n < m) {
    throw std::invalid_argument("There are not enough points");
  }
  if (n == 0) {
    throw std::invalid_argument("Tables are empty");
    return 0.0;
  }
  //find nodes satisfying the condition of interpolation
  int h = 0;
  for (int i = 0; i < n; i++) {
    if ((arrPoints[i] > x) && (i >= m)) {
        h = i + 1 - m;
        break;
    }
  }
  //count
  double q = 0.0;
  for (int i = h; i < h + m; i++) {
    double p = 1.0;
    for (int j = h; j < h + m; j++) {
      if (j != i) {
        p *= x - arrPoints[j];
        p /= arrPoints[i] - arrPoints[j];
      }
    }
    p *= arrFunctions[i];
    q += p;
  }

  return q;
}
