// Workshop 5 - Matrix Multiplication - Tasks
// MatMul.worksharing.h
// 2018.10.08
// Chris Szalwinski

#pragma once
#define N 2048
#define M 16
typedef double array[N];

void initialize(double a[][N]);
void matmul(const double a[][N], const double b[][N], double c[][N], int n);
double checksum(const double x[][N]);