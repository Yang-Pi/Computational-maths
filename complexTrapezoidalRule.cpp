#include <iostream>
#include <math.h>

double function(double);
void TRAP(double (*funcPtr)(double), const double, const double, const double, unsigned int&, double&);

int main(int argc, char* argv[])
{
  double (*funcPtr)(double) = &function;

  const double A = -10.0;
  const double B = 10.0;
  const double EPS = 0.01;

  unsigned int N = 0;
  double result = 0.0;
  TRAP(funcPtr, A, B, EPS, N, result);

  std::cout << result << "; " << N << '\n';
}

double function(double value)
{
  double part1 = 5 * pow(value, 2);
  double part2 = value;
  double part3 = cos(value);
  return (part1 + part2) + part3;
}

void TRAP(double (*funcPtr)(double), const double a, const double b,
          const double eps, unsigned int& nResult, double& result)
{
  const double aCounted = (*funcPtr)(a);
  const double bCounted = (*funcPtr)(b);
  //std::cout << "A = " << aCounted << " B = " << bCounted << "\n";
  double sOld = 0.0;
  double sNew = 0.5 * (b - a) * (aCounted + bCounted);
  unsigned int n = 1;
  std::cout << sNew << "\n";

  do {
    sOld = sNew;
    n *= 2;
    double h = (b - a) / n;
    double s = 0.0;

    for (int k = 1; k < n; ++k) {
      double value = a + k * h;
      s = s + (*funcPtr)(value);
    }

    sNew = (b - a) / (2 * n) * ( aCounted + 2 * s + bCounted );
    std::cout << "SNEW = " << sNew << ";   N = " << n <<'\n';
  } while ( abs(sOld - sNew) > eps );

  result = sNew;
  nResult = n;
}
