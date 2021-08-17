//// Workshop 8 - Domain Decomposition
//// based on code from LLNL tutorial mpi_heat2d.c
//// Master-Worker Programming Model
//// Chris Szalwinski - 2018/11/13
//// w8.simple.cpp
//#include <iostream>
//#include <fstream>
//#include <iomanip>
//#include <cstdlib>
//#include <chrono>
//using namespace std::chrono;
//
//void initialize(int, int, float*);
//void update(int, int, int, const float, const float, const float*, float*);
//void output(int, int, const float*, const char*);
//
//// report system time
////
//void reportTime(const char* msg, steady_clock::duration span) {
//   auto ms = duration_cast<milliseconds>(span);
//   std::cout << msg << " - took - " <<
//      ms.count() << " milliseconds" << std::endl;
//}
//
//// weights
//const float wx = 0.1f;
//const float wy = 0.1f;
//
//int main(int argc, char** argv) {
//   if (argc != 4) {
//      std::cerr << "*** Incorrect number of arguments ***\n";
//      std::cerr << "Usage: " << argv[0]
//         << " no-of_rows no_of_columns no_of_iterations\n";
//      return 1;
//   }
//
//   // grid size
//   int nRows = std::atoi(argv[1]);
//   int nColumns = std::atoi(argv[2]);
//   int nIterations = std::atoi(argv[3]);
//
//   // allocate memory for decomposition
//   steady_clock::time_point ts, te;
//   float* data;
//   try {
//      // 2 * for double buffering
//      data = new float[2 * nRows * nColumns];
//   }
//   catch (std::bad_alloc) {
//      std::cerr << "*** Failed to Allocate Memory for 2 * "
//         << nRows << " by " << nColumns << " grid" << std::endl;
//      return 3;
//   }
//
//   // initialize first buffer data[0: nRowsTotal*nColumns)
//   initialize(nRows, nColumns, data);
//   // initialize second buffer data[nRowsTotal*nColumns : nRowsTotal*nColumns)
//   for (int i = 0; i < nRows * nColumns; i++) data[nRows * nColumns + i] = 0.0f;
//
//   // write original data to file
//   output(nRows, nColumns, data, "original.dat");
//
//   // decompose the data and iterate on each partition
//   ts = steady_clock::now();
//   int iz = 0;
//   for (int i = 0; i < nIterations; i++) {
//      std::cout << "Iteration " << i + 1 << std::endl;
//      int rowOffset = 0;
//      update(1, nRows - 2, nColumns, wx, wy,
//         data + iz * nRows * nColumns, data + (1 - iz) * nRows * nColumns);
//      // switch buffers
//      iz = 1 - iz;
//   }
//   te = steady_clock::now();
//   reportTime("\nw8 Domain Decomposition ", te - ts);
//   std::cout << "Number of Rows       " << nRows << std::endl;
//   std::cout << "Number of Columns    " << nColumns << std::endl;
//
//   // write results to file
//   output(nRows, nColumns, data, "results.dat");
//
//   // checksum
//   double sum1 = 0.0, sum2 = 0.0;
//   for (int i = 0; i < nRows * nColumns; ++i) {
//      sum1 += data[i];
//      sum2 += data[i + nRows * nColumns];
//   }
//   std::cout << "Checksums = " << sum1 << ',' << sum2 << std::endl;
//
//   // deallocate
//   delete[] data;
//}
//
//// initialize for high value at above end middle row
////
//void initialize(int nRowsTotal, int nColumns, float* x) {
//   for (int i = 0; i < nRowsTotal; i++)
//      for (int j = 0; j < nColumns; j++)
//         x[i * nColumns + j] = (float)(i * (nRowsTotal - i - 1) * j
//            * (nColumns - j - 1));
//}
//
//// update data using weighted neighboring values
////
//void update(int startRow, int endRow, int nColumns, const float wx,
//   const float wy, const float* x_old, float* x_new) {
//   for (int i = startRow; i <= endRow; i++)
//      for (int j = 1; j < nColumns - 1; j++)
//         x_new[i * nColumns + j] = x_old[i * nColumns + j]
//         + wx * (x_old[(i + 1) * nColumns + j] + x_old[(i - 1) * nColumns + j]
//            - 2.0f * x_old[i * nColumns + j])
//         + wy * (x_old[i * nColumns + j + 1] + x_old[i * nColumns + j - 1]
//            - 2.0f * x_old[i * nColumns + j]);
//}
//
//// output data
////
//void output(int nRowsTotal, int nColumns, const float* x, const char* filename) {
//   std::ofstream file(filename);
//   file << std::fixed << std::setprecision(1);
//   for (int j = nColumns - 1; j >= 0; j--) {
//      for (int i = 0; i < nRowsTotal; i++)
//         file << std::setw(8) << x[i * nColumns + j];
//      file << std::endl;
//   }
//}