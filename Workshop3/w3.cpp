//// Workshop 3 - Matrix Multiplication - Optimization
// // w3.cpp
// // 2018.06.27
// // Chris Szalwinski
//
//#include <iostream>
//#include <iomanip>
//#include <cstdlib>
//#include <chrono>
//#include "MatMul.h"
//
//void reportTime(const char* msg, std::chrono::steady_clock::duration span);
//
//int main(int argc, char** argv) {
//   if (argc != 1) {
//      std::cerr << "*** Incorrect number of arguments ***\n";
//      std::cerr << "Usage: " << argv[0] << "\n";
//      return 1;
//   }
//   std::cout << std::fixed << std::setprecision(2);
//
//   // allocate memory for matrices
//   double* a = new double[N * N];
//   double* b = new double[N * N];
//   double* c = new double[N * N];
//   array* aa = (array*)a;
//   array* bb = (array*)b;
//   array* cc = (array*)c;
//   std::cout << "Matrix Multiplication " << N << " by " << N << std::endl;
//
//   // initialize a and b
//   std::chrono::steady_clock::time_point ts, te;
//   ts = std::chrono::steady_clock::now();
//   initialize(aa);
//   initialize(bb);
//   double* t = c;
//   for (int i = 0; i < N * N; i++)
//      *t++ = 0.0;
//   te = std::chrono::steady_clock::now();
//   reportTime("initialization", te - ts);
//
//   // User selects upgrade
//   int upgrade = 0;
//   bool ok = false;
//   do {
//      std::cout << "\n1. Baseline\n2. Loop-Interchange\n3. OpenMP" << std::endl;
//      std::cout << "Please enter the type of upgrade from 1 - 3: ";
//      if (!(std::cin >> upgrade)) {
//         ok = false; std::cin.clear(); std::cin.ignore(123, '\n');
//      }
//      else if (upgrade > 4 || upgrade < 1) 
//         ok = false;
//      else 
//         ok = true;
//   } while (!ok && std::cout << "Please enter a number between '1 - 3'\n");
//   std::cout << std::endl;
//
//   // Multiply matrices a and b
//   if (upgrade == 1) {
//      ts = std::chrono::steady_clock::now();
//      matmul1(aa, bb, cc, N);
//      te = std::chrono::steady_clock::now();
//   }
//   else if (upgrade == 2) {
//      ts = std::chrono::steady_clock::now();
//      matmul2(aa, bb, cc, N);
//      te = std::chrono::steady_clock::now();
//   }
//   else if (upgrade == 3) {
//      ts = std::chrono::steady_clock::now();
//      matmul3(aa, bb, cc, N);
//      te = std::chrono::steady_clock::now();
//   }
//   else if (upgrade == 4) {
//      ts = std::chrono::steady_clock::now();
//      matmul4(aa, bb, cc, N);
//      te = std::chrono::steady_clock::now();
//   }
//
//   // multiply matrices a and b
//   /*ts = std::chrono::steady_clock::now();
//   matmul(aa, bb, cc, N);
//   te = std::chrono::steady_clock::now();*/
//   reportTime("execution", te - ts);
//   std::cout << "checksum = " << checksum(cc) << std::endl;
//
//   // deallocate memory
//   delete[] a;
//   delete[] b;
//   delete[] c;
//}
//
//// reportTime inserts the duration (span) into standard output
////
//void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
//   auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(span);
//   std::cout << msg << ": " <<
//      ms.count() << " milliseconds" << std::endl;
//}


// Workshop 3 - Matrix Multiplication - Optimization
 // w3.cpp
 // 2020.09.30
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
      std::cerr << "Usage: " << argv[0]
         << " rows_in_a columns_in_a columns_in_b\n";
      return 1;
   }
   std::cout << std::fixed << std::setprecision(2);

   // allocate memory for matrices
   std::chrono::steady_clock::time_point ts, te;

   // matrix ceofficient stored in row major order
   double* a = new double[N * N];
   double* b = new double[N * N];
   double* c = new double[N * N]{ 0 };
   // addresses of the rows in each matrix
   row* aa = reinterpret_cast<row*>(a);
   row* bb = reinterpret_cast<row*>(b);
   row* cc = reinterpret_cast<row*>(c);

   std::cout << "Matrix Multiplication " << N << " by " << N << std::endl;

   // initialize a and b
   ts = std::chrono::steady_clock::now();
   initialize(aa);
   initialize(bb);
   te = std::chrono::steady_clock::now();
   reportTime("initialization", te - ts);

   //// matrix multiply cc = aa * bb
   //ts = std::chrono::steady_clock::now();
   //matmul(aa, bb, cc, N);
   //te = std::chrono::steady_clock::now();
   //reportTime("execution", te - ts);


    // User selects upgrade
   int upgrade = 0;
   bool ok = false;
   do {
      std::cout << "\n1. Baseline\n2. OpenMP\n3. Loop-Interchange\n4. OpenMP + Loop-Interchange\n" << std::endl;
      std::cout << "Please enter the type of upgrade from 1 - 4: ";
      if (!(std::cin >> upgrade)) {
         ok = false; std::cin.clear(); std::cin.ignore(123, '\n');
      }
      else if (upgrade > 5 || upgrade < 1) 
         ok = false;
      else 
         ok = true;
   } while (!ok && std::cout << "Please enter a number between '1 - 4'\n");
   std::cout << std::endl;

   // Multiply matrices a and b
   if (upgrade == 1) {
      ts = std::chrono::steady_clock::now();
      matmul1(aa, bb, cc, N);
      te = std::chrono::steady_clock::now();
      reportTime("execution", te - ts);
   }
   else if (upgrade == 2) {
      ts = std::chrono::steady_clock::now();
      matmul2(aa, bb, cc, N);
      te = std::chrono::steady_clock::now();
      reportTime("execution", te - ts);
   }
   else if (upgrade == 3) {
      ts = std::chrono::steady_clock::now();
      matmul3(aa, bb, cc, N);
      te = std::chrono::steady_clock::now();
      reportTime("execution", te - ts);
   }
   else if (upgrade == 4) {
      ts = std::chrono::steady_clock::now();
      matmul4(aa, bb, cc, N);
      te = std::chrono::steady_clock::now();
      reportTime("execution", te - ts);
   }
   else if (upgrade == 5) {
      ts = std::chrono::steady_clock::now();
      matmul5(aa, bb, cc, N);
      te = std::chrono::steady_clock::now();
      reportTime("execution", te - ts);
   }

   std::cout << "checksum = " << checksum(cc) << std::endl;

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