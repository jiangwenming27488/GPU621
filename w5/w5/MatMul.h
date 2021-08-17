 // Workshop 5 - Matrix Multiplication - Tasks - Baseline
 // MatMul.h
 // 2018.10.27
 // Chris Szalwinski

 #pragma once
 #include <iostream>
 constexpr size_t N = 2048;
 constexpr size_t M = 16;
 using row = double[N];

 void initialize(row a[]);
 void matmul(const row a[], const row b[], row c[], int n);
 double checksum(const row x[]);