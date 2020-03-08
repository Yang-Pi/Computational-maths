#include <iostream>

//LAGRANGE
double lagrange(int n, double arrPoints[], double arrFunctions[], int m, double x)
{
  if (n < m) {
    throw std::invalid_argument("There are not enough points");
  }
  if (n <= 0) {
    throw std::invalid_argument("Tables are empty");
  }
  //find nodes satisfying the condition of interpolation
  int h = 0;
  for (int i = 0; i < n; i++) {
    if ((arrPoints[i] > x) && (i >= m)) {
        h = i + 1 - m;
        break;
    }
  }
  //compute
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