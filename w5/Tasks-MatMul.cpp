// Workshop 5 - Matrix Multiplication - Tasks - Baseline
// MatMul.cpp
// 2018.10.27
// Chris Szalwinski

#include "MatMul.h"
#include <omp.h>

/* Untiled Version
*/
// matmul returns the product c = a * b
//
//void matmul(const row a[], const row b[], row c[], int n) {
//   {
//            for (int i = 0; i < n; i++) {
//               for (int k = 0; k < n; k++) { 
//                  for (int j = 0; j < n; j++) {
//                        c[i][j] += a[i][k] * b[k][j];
//                  }
//               }
//            }
//   }
//}
// End Untiled Version

/* Tiled Version
*/
// matmul returns the product c = a * b
//
void matmul(const row a[], const row b[], row c[], int n) {
#pragma omp parallel
   {
#pragma omp single
      {
         
         for (int i = 0; i < n; i+=M) {
            for (int k = 0; k < n; k+=M) {
               for (int j = 0; j < n; j+=M) {
#pragma omp task depend(in: a[i:M][k:M], b[k:M][j:M])
                  for (int ii = i; ii < i + M; ii++) {
#pragma omp task depend(in: a[i:M][k:M], b[k:M][j:M])
                     for (int kk = k; kk < k + M; kk++) {
#pragma omp task depend(in: a[i:M][k:M], b[k:M][j:M]) depend(out: c[i:M][j:M])
                        {
                           #pragma omp simd
                              for (int jj = j; jj < j + M; jj++) {
                                    c[ii][jj] += a[ii][kk] * b[kk][jj];
                              }
                        }
                     
                     }
                  }
               }
            }
         }

      }
   }

}
// End Tiled Version

// checksum returns the sum of the coefficients in matrix x[rows][cols]
//
double checksum(const row x[]) {

   double sum = 0.0;
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

   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
            a[i][j] = 0.0;
      }
      a[i][i] = 1.0;
   }
}