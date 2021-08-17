#include <iostream>
#include <cstdio>
#include <omp.h>
//#include "../../../../../Program Files (x86)/IntelSWTools/compilers_and_libraries_2020/windows/compiler/include/omp.h"

int main() {
   int isHost = 0;

#pragma omp target map(from: isHost)
   { isHost = omp_is_initial_device(); }

   if (isHost < 0) {
      printf("Runtime error, isHost=%d\n", isHost);
   }

   // CHECK: Target region executed on the device
   printf("Target region executed on the %s\n", isHost ? "host" : "device");

   return isHost;


   //int x = 1;

//#pragma omp parallel
//   {
//      std::cout << omp_get_num_threads() << std::endl;
//
//   }

//#pragma omp target map(tofrom: x)
//   {
//      x = x + 1;
//   }
//   printf("After target region is executed: %d\n", x);

//#pragma omp target map(to:x) map(tofrom:y)
//   {
//#pragma omp parallel
//      {
//         x = x + 1;
//      }
//   }
//   printf("After target region is executed: %d\n", x);

  // return 0;
}