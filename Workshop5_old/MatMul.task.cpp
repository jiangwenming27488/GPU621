// Workshop 5 - Matrix Multiplication - Tasks
// MatMul.task.cpp
// 2018.10.08
// Chris Szalwinski

#include <omp.h>
#include "MatMul.h"

// matmul returns the product c = a * b
//
void matmul(const double a[][N], const double b[][N], double c[][N], int n) {
#pragma omp parallel 
   {

#pragma omp single 
      {
         for (int i = 0; i < n; i += M) {
            for (int k = 0; k < n; k += M) {
               for (int j = 0; j < n; j += M) {
                  for (int ii = i; ii < i + M; ii++) {
#pragma omp task depend(out: ii)
                     for (int kk = k; kk < k + M; kk++) {
#pragma omp task depend(in: ii) depend(out: kk)
                        for (int jj = j; jj < j + M; jj++) {
#pragma omp task depend(in: ii, kk) depend(inout: jj) 
                           c[ii][jj] += a[ii][kk] * b[kk][jj]; // add simd later
                        }
                     }
                  }
               }
            }
         }

      }

   }


}

// checksum returns the sum of the coefficients in matrix x[N][N]
//
double checksum(const double x[][N]) {

   double sum = 0.0;
#pragma omp parallel for collapse(2) reduction(+:sum)
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         sum += x[i][j];
      }
   }
   return sum;
}

// initialize initializes matrix a[N][N]
//
void initialize(double a[][N]) {

#pragma omp parallel for
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         a[i][j] = static_cast<double>(i * j) / (N * N);
      }
   }
}