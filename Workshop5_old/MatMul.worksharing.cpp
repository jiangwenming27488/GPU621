//// Workshop 5 - Matrix Multiplication - Tasks
// // MatMul.worksharing.cpp
// // 2018.10.08
// // Chris Szalwinski
//
//#include <omp.h>
//#include "MatMul.h"
//
//// matmul returns the product c = a * b
////
//void matmul(const double a[][N], const double b[][N], double c[][N], int n) {
//#pragma omp parallel for collapse(2)
//   for (int i = 0; i < n; i++) {
//      for (int j = 0; j < n; j++) {
//         for (int k = 0; k < n; k++) {
//            c[i][j] += a[i][k] * b[k][j];
//         }
//      }
//   }
//
//
////   // Loop interchange 
////#pragma omp parallel for collapse(2)
////   for (int i = 0; i < n; i++) {
////      for (int k = 0; k < n; k++) {
////         for (int j = 0; j < n; j++) {
////            c[i][j] += a[i][k] * b[k][j];
////         }
////      }
////   }
//
//   // Vectorized
////#pragma omp parallel for collapse(2)
////   for (int i = 0; i < n; i++) {
////      for (int k = 0; k < n; k++) {
////#pragma omp simd
////         for (int j = 0; j < n; j++) {
////            c[i][j] += a[i][k] * b[k][j];
////         }
////      }
////   }
//
//
//}
//
//// checksum returns the sum of the coefficients in matrix x[N][N]
////
//double checksum(const double x[][N]) {
//
//   double sum = 0.0;
//#pragma omp parallel for collapse(2) reduction(+:sum)
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
//
//#pragma omp parallel for
//   for (int i = 0; i < N; i++) {
//      for (int j = 0; j < N; j++) {
//         a[i][j] = static_cast<double>(i * j) / (N * N);
//      }
//   }
//}