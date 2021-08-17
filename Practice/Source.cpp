#include <iostream>
#include <omp.h>


int main()
{
   constexpr int n{ 5 };

   int x[]{ 1,2,1,2,1 }, y[n];

   int p = 1;
#pragma omp simd reduction(inscan, *:p)
   for (auto i = 0; i < n; ++i) {
#pragma omp scan inclusive(p)
      y[i] = p;
#pragma omp scan exclusive(p)
      p *= x[i];

   }

   for (auto i = 0; i < n; ++i)
      std::cout << y[i] << ' ';

     std::cout << '\n';


#pragma omp parallel for
     for (auto i = 0; i < n; i <<= 2) {
        std::cout << y[i] << std::endl;
     }
}