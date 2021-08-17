//// Workshop 9 - Discretize the Elements of an Array
// // Chris Szalwinski
// // 2018/11/18
// // w9.serial.c
//
//#include "stdio.h"
//#include "stdlib.h"
//#include "math.h"
//#include "time.h"
//
//// report processor time
////
//void reportTime(const char* msg, double span) {
//   printf("%-14s took %7.4lf seconds\n", msg, span);
//}
//
//// discretize data[n] into 0s and 1s
////
//void discretize(float* data, int n) {
//   for (int i = 0; i < n; i++)
//      data[i] = (pow(sin(data[i]), cos(data[i])) +
//         pow(cos(data[i]), sin(data[i]))) / 2.0f;
//}
//
//int main(int argc, char** argv) {
//   if (argc != 2) {
//      fprintf(stderr, "%s : invalid number of arguments\n"
//         "Usage: %s no_of_elements\n", argv[0], argv[0]);
//      return 1;
//   }
//
//   // retrieve number of elements
//   int n = atoi(argv[1]);
//   clock_t start, end;
//
//   // initialization
//   start = clock();
//   float* data = (float*)malloc(n * sizeof(float));
//   for (int i = 0; i < n; i++)
//      data[i] = (float)rand() / RAND_MAX;
//   end = clock();
//   reportTime("Initialization", (double)(end - start) / CLOCKS_PER_SEC);
//
//   // discretization and collation
//   start = clock();
//   discretize(data, n);
//   end = clock();
//   int time = end - start;
//   reportTime("Conversion", (double)time / CLOCKS_PER_SEC);
//   int zeros = 0;
//   for (int i = 0; i < n; i++)
//      if (data[i] < 0.707f)
//         zeros++;
//
//   // report
//   printf("%d = %d 0s + %d 1s\n", n, zeros, n - zeros);
//
//   // clean up
//   free(data);
//}