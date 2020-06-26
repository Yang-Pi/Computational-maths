#include <iostream>

int main()
{
  double x = 1.0;
  double a = 1;
  double y = x + a;

  while (y != x) {
    a /= 2;
    y = x + a;
    std::cout << a << "\n";
  }
}
