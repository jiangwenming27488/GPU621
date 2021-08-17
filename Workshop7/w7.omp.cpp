//// Workshop 7 - Prefix Scan Comparison
// // w7.omp.cpp
// // 2018.11.04
// // Chris Szalwinski
//
//#include <iostream>
//#include <chrono>
//#include "OMP-Body.h"
//
//// report system time
////
//void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
//   auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(span);
//   std::cout << msg << " - took - " <<
//      ms.count() << " milliseconds" << std::endl;
//}
//
//int main(int argc, char** argv) {
//   if (argc > 2) {
//      std::cerr << argv[0] << ": invalid number of arguments\n";
//      std::cerr << "Usage: " << argv[0] << "\n";
//      std::cerr << "Usage: " << argv[0] << "  power_of_2\n";
//      return 1;
//   }
//   std::cout << "OpenMP Prefix Scan" << std::endl;
//
//   // initial values for testing
//   const int N = 8;
//   const int in_[N]{ 3, 1, 7, 0, 1, 4, 5, 9 };
//
//   // command line arguments - none for testing, 1 for large arrays
//   int n;
//   if (argc == 1) {
//      n = N;
//   }
//   else {
//      n = 1 << std::atoi(argv[1]);
//      if (n < N) n = N;
//   }
//   int* in = new int[n];
//   int* out = new int[n];
//
//   // initialize
//   for (int i = 0; i < N; i++)
//      in[i] = in_[i];
//   for (int i = N; i < n; i++)
//      in[i] = 1;
//   auto add = [](int a, int b) { return a + b; };
//
//   // Exclusive Prefix Scan
//   std::chrono::steady_clock::time_point ts, te;
//   ts = std::chrono::steady_clock::now();
//   scan(in, out, n, reduce<int, decltype(add)>,
//      add, incl_scan<int, decltype(add)>, (int)0);
//   te = std::chrono::steady_clock::now();
//   std::cout << "n = " << n << ": ";
//   for (int i = 0; i < N; i++)
//      std::cout << out[i] << ' ';
//   std::cout << out[n - 1] << std::endl;
//   reportTime("Inclusive Prefix Scan", te - ts);
//
//   delete[] in;
//   delete[] out;
//}