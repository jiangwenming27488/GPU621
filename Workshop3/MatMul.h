//// Workshop 3 - Matrix Multiplication - Auto Vectorization
//// MatMul.h
//// 2018.06.27
//// Chris Szalwinski
//
//#pragma once
//#define N 2048
//typedef double array[N];
//
//void initialize(double a[][N]);
//void matmul1(const double a[][N], const double b[][N], double c[][N], int n);
//void matmul2(const double a[][N], const double b[][N], double c[][N], int n);
//void matmul3(const double a[][N], const double b[][N], double c[][N], int n);
//void matmul4(const double a[][N], const double b[][N], double c[][N], int n);
//double checksum(const double x[][N]);



// Workshop 3 - Matrix Multiplication - Auto Vectorization
// MatMul.h
// 2020.09.30
// Chris Szalwinski
#pragma once

constexpr size_t N{ 2048 };
using row = double[N];

void initialize(row a[]);
void matmul1(const row a[], const row b[], row c[], int n);
void matmul2(const row a[], const row b[], row c[], int n);
void matmul3(const row a[], const row b[], row c[], int n);
void matmul4(const row a[], const row b[], row c[], int n);
void matmul5(const row a[], const row b[], row c[], int n);
double checksum(const row x[]);
void initialize(row a[]);
