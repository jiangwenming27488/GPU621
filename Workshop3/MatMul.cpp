// Workshop 3 - Matrix Multiplication - Optimization
 // MatMul.cpp
 // 2018.06.27
 // Chris Szalwinski

#include <omp.h>
#include <iostream>
#include "MatMul.h"

// matmul returns the product c = a * b
//

// Baseline 
void matmul1(const row a[], const row b[], row c[], int n) {
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         for (int k = 0; k < n; k++) {
            c[i][j] += a[i][k] * b[k][j];
         }
      }
   }

}

// OpenMP
void matmul2(const row a[], const row b[], row c[], int n) {
#pragma omp parallel for
   for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
         for (int k = 0; k < n; k++) {
            c[i][j] += a[i][k] * b[k][j];
         }
      }
   }
}

// Loop interchange
void matmul3(const row a[], const row b[], row c[], int n) {
  for (int i = 0; i < n; i++) {
     for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
           c[i][j] += a[i][k] * b[k][j];
        }
     }
  }
}

// OpenMP with loop interchange 
void matmul4(const row a[], const row b[], row c[], int n) {
#pragma omp parallel for
   for (int i = 0; i < n; i++) {
      for (int k = 0; k < n; k++) {
         for (int j = 0; j < n; j++) {
            c[i][j] += a[i][k] * b[k][j];
         }
      }
   }
}

// Cache Blocking with loop interchange and OpenMP
void matmul5(const row a[], const row b[], row c[], int n) {
   const int blockSize = (int)N / 8;
#pragma omp parallel for
   for (int i = 0; i < n; i += blockSize) {
      for (int k = 0; k < n; k += blockSize) {
         for (int j = 0; j < n; j += blockSize) {
            for (int ib = i; ib < i + blockSize; ib++) {
               for (int kb = k; kb < k + blockSize; kb++) {
                  for (int jb = j; jb < j + blockSize; jb++) {
                     c[ib][jb] += a[ib][kb] * b[kb][jb];
                  }
               }
            }
         }
      }
   }

}


// checksum returns the sum of the coefficients in matrix x[rows][cols]
//
double checksum(const row x[]) {

   double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         sum += x[i][j];
      }
   }
   return sum;
}

// initialize initializes matrix a[rows][cols]
//
void initialize(row a[]) {
#pragma omp parallel for
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         a[i][j] = static_cast<double>(i * j) / (N * N);
      }
   }
}


//// checksum returns the sum of the coefficients in matrix x[N][N]
////
//double checksum(const double x[][N]) {
//
//   double sum = 0.0;
//#pragma omp parallel for reduction (+:sum)
//   for (int i = 0; i < N; i++) {
//      for (int j = 0; j < N; j++) {
//         sum += x[i][j];
//      }
//   }
//   return sum;
//}
//
//// initialize initializes matrix a[N][N]
////
//void initialize(double a[][N]) {
//#pragma omp parallel for
//   for (int i = 0; i < N; i++) {
//      for (int j = 0; j < N; j++) {
//         a[i][j] = static_cast<double>(i * j) / (N * N);
//      }
//   }
//}

