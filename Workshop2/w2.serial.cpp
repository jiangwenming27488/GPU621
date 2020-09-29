// Workshop 2 - Calculate PI by integrating 1/(1+x^2)

// Nathan Olah
// Student Number: 124723198
// nolah@myseneca.ca
// 2020-09-28

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include <string>
using namespace std::chrono;

#define NUM_THREADS 8
#define PAD 8 // 64 byte L1 cache line size 

void reportTime(const char* msg, steady_clock::duration span);

std::string getVersion() {
   std::string str;
   bool ok = false;
   std::string methods[] = {"naive", "padded", "sync", "synchronized"};
   std::cout << "Enter the version 'naive', 'padded', or 'synchronized' : ";
   do {
       std::cin >> str;
       std::cin.clear();
       for (int i = 0; i < sizeof(methods) / sizeof(methods[0]); i++) {
          if (str.compare(methods[i]) == 0) {
             ok = true;     
             break;
          }
       }
   } while (!ok && std::cout << "invalid method" << std::endl);

   return str;
}

// Naive Version
void naive(int n) {
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i, nthreads;
   double sum[NUM_THREADS];

#pragma omp parallel 
   {
      int i, nthrds, id;
      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) nthreads = nthrds;
      for (i = id, sum[id] = 0.0; i < n; i = i + nthrds) {
         x = ((double)i + 0.5) * stepSize;
         sum[id] += 1.0 / (1.0 + x * x);
      }
   }

   // sum up the values
   for (i = 0, pi = 0.0; i < nthreads; i++) {
      pi += 4.0 * sum[i] * stepSize;
   }

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

} 

// Padded Version
void padded(int n) {
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i, nthreads;
   double sum[NUM_THREADS][PAD];

#pragma omp parallel
   {
      int i, nthrds, id;
      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) nthreads = nthrds;
      for (i = id, sum[id][0] = 0.0; i < n; i = i + nthrds) {
         x = ((double)i + 0.5) * stepSize;
         sum[id][0] += 1.0 / (1.0 + x * x);
      }
   }

   // sum up the values
   for (i = 0, pi = 0.0; i < nthreads; i++) {
      pi += 4.0 * sum[i][0] * stepSize;
   }

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);
}

// Synchronized Version
void synchronized(int n) {
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i;
   double sum = 0;

   int nthreads;
#pragma omp parallel
   {
      int i, id, nthrds;
      double x, sum;

      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) {
         nthreads = nthrds;
      }

      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();

      for (i = id, sum = 0.0; i < n; i = i + nthreads) {
         x = ((double)i + 0.5) * stepSize;
         sum += 4.0 / (1.0 + x * x);
      }

      // Mutual Exclusion
#pragma omp critical
      pi += sum * stepSize;

   }

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

}

// report system time
//
void reportTime(const char* msg, steady_clock::duration span) {
   auto ms = duration_cast<milliseconds>(span);
   std::cout << msg << " - took - " <<
      ms.count() << " milliseconds" << std::endl;
}

int main(int argc, char** argv) {
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
      return 1;
   }
   int n = std::atoi(argv[1]);

   // select version of openmp 
   std::string version = getVersion();

   if (version.compare("naive") == 0) {
      naive(n);
   }
   else if (version.compare("padded") == 0) {
      padded(n);
   }
   else if (version.compare("synchronized") == 0 || version.compare("sync") == 0) {
      synchronized(n);
   }
   else {
      std::cout << "No version found" << std::endl;
   }

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);

   return 0;
}












//////////////////////////////////////////////////////////////////////////////

// Synchronized Version
int _____main(int argc, char** argv) {
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
      return 1;
   }
   int n = std::atoi(argv[1]);
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i;
   double sum = 0;

//#pragma omp parallel for reduction(+:sum)
//   for (i = 0; i < n; i++) {
//      x = ((double)i + 0.5) * stepSize;
//      sum += 1.0 / (1.0 + x * x);
//   }

   int nthreads;
#pragma omp parallel
   {

      int i, id, nthrds;
      double x, sum;

      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) {
         nthreads = nthrds;
      }

      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();

      std::cout << "Number of threads : " << nthrds << std::endl;

      for (i = id, sum = 0.0; i < n; i = i + nthreads) {
         x = ((double)i + 0.5) * stepSize;
         sum += 4.0 / (1.0 + x * x); 
      }

      // Mutual Exclusion
#pragma omp critical
      pi += sum * stepSize;

      
   }
  
   // use this with reduction
   //pi = 4.0 * sum * stepSize;

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);

   return 0;
}

// Padded Version
int _________main(int argc, char** argv) {
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
      return 1;
   }
   int n = std::atoi(argv[1]);
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i, nthreads;
   double sum[NUM_THREADS][PAD];

#pragma omp parallel
   {
      int i, nthrds, id;
      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) nthreads = nthrds;
      for (i = id, sum[id][0] = 0.0; i < n; i = i + nthrds) {
         x = ((double)i + 0.5) * stepSize;
         sum[id][0] += 1.0 / (1.0 + x * x);
      }
   }

   // sum up the values
   for (i = 0, pi = 0.0; i < nthreads; i++) {
      pi += 4.0 * sum[i][0] * stepSize;
   }

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);


   return 0;
}

// Naive Version
int _________________main(int argc, char** argv) {
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
      return 1;
   }
   int n = std::atoi(argv[1]);
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi = 0.0;  
   double stepSize = 1.0 / (double)n;

   omp_set_num_threads(NUM_THREADS);
   int i, nthreads;
   double sum[NUM_THREADS];

#pragma omp parallel 
   {
      int i, nthrds, id;
      id = omp_get_thread_num();
      nthrds = omp_get_num_threads();
      if (id == 0) nthreads = nthrds;
      for (i = id, sum[id] = 0.0; i < n; i = i + nthrds) {
         x = ((double)i + 0.5) * stepSize;
         sum[id] += 1.0 / (1.0 + x * x);
      } 
   }

   // sum up the values
   for (i = 0, pi = 0.0; i < nthreads; i++) {
      pi += 4.0 * sum[i] * stepSize;
   }

   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);
   
   return 0;
}

// Serial Version
int __main(int argc, char** argv) {
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
      return 1;
   }
   int n = std::atoi(argv[1]);
   steady_clock::time_point ts, te;

   // calculate pi by integrating the area under 1/(1 + x^2) in n steps 
   ts = steady_clock::now();
   double x, pi, sum = 0.0;
   double stepSize = 1.0 / (double)n;
   for (int i = 0; i < n; i++) {
      x = ((double)i + 0.5) * stepSize;
      sum += 1.0 / (1.0 + x * x);
   }
   pi = 4.0 * sum * stepSize;
   te = steady_clock::now();

   std::cout << "n = " << n <<
      std::fixed << std::setprecision(15) <<
      "\n pi(exact) = " << 3.141592653589793 <<
      "\n pi(calcd) = " << pi << std::endl;
   reportTime("Integration", te - ts);

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);
}