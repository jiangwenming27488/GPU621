// Workshop 5 - Matrix Multiplication - Tasks
 // w5.cpp
 // 2018.10.08
 // Chris Szalwinski

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include "MatMul.h"

void reportTime(const char* msg, std::chrono::steady_clock::duration span);

int main(int argc, char** argv) {
   if (argc != 1) {
      std::cerr << "*** Incorrect number of arguments ***\n";
      std::cerr << "Usage: " << argv[0] << "\n";
      return 1;
   }
   std::cout << std::fixed << std::setprecision(2);

   // allocate memory for matrices
   double* a = new double[N * N];
   double* b = new double[N * N];
   double* c = new double[N * N];
   array* aa = (array*)a;
   array* bb = (array*)b;
   array* cc = (array*)c;
   std::cout << "Matrix Multiplication " << N << " by " << N << std::endl;

   // initialize a and b
   std::chrono::steady_clock::time_point ts, te;
   ts = std::chrono::steady_clock::now();
   initialize(aa);
   initialize(bb);
   double* t = c;
   for (int i = 0; i < N * N; i++)
      *t++ = 0.0;
   te = std::chrono::steady_clock::now();
   reportTime("initialization", te - ts);

   // multiply matrices a and b
   ts = std::chrono::steady_clock::now();
   matmul(aa, bb, cc, N);
   te = std::chrono::steady_clock::now();
   reportTime("execution", te - ts);
   std::cout << "checksum = " << checksum(cc) << std::endl;

   // deallocate memory
   delete[] a;
   delete[] b;
   delete[] c;
}

// reportTime inserts the duration (span) into standard output
//
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
   auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(span);
   std::cout << msg << ": " <<
      ms.count() << " milliseconds" << std::endl;
}