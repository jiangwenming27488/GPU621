// Workshop 1 - Platforms and Optimizations
// w1.cpp

#include <iostream>
#include <cmath>
#include <chrono>
#include <string>
using namespace std::chrono;

class Version {
public:
   void operator()() const {
      std::cout << "Hello from the ";
#if defined(__GNUC__)           // for GCC 5.2.0
      std::cout << "GNU C/C++ compiler: ";   // Insert compiler name
      std::cout << __GNUC__ << "." << __GNUC_MINOR__ << "." << __INCLUDE_LEVEL__ << "\n"; // Insert version.revision.patch 

#elif defined(__INTEL_COMPILER)         // for Intel Parallel Studio
      std::cout << "Intel C/C++ compiler: ";   // Insert compiler name
      std::string str = std::to_string(__INTEL_COMPILER);
      std::cout << str.substr(0, 2) << "." << str.substr(2, 1) << "." << str.substr(3, 1) << "\n"; // Insert version.revision.patch 

#elif defined(_MSC_FULL_VER)         // for Visual Studio
      std::cout << "Microsoft Visual C++ compiler: ";   // Insert compiler name
      std::string str = std::to_string(_MSC_FULL_VER);
      std::cout << str.substr(0, 2) << "." << str.substr(2, 2) << "." << str.substr(4, 5) << "\n"; // Insert version.revision.patch 
#else
      std::cout << "??? compiler: ";   // None of the above
#endif
   }
};

// report system time
//
void reportTime(const char* msg, steady_clock::duration span) {
   auto ms = duration_cast<milliseconds>(span);
   std::cout << msg << " - took - " <<
      ms.count() << " milliseconds" << std::endl;
}

double magnitude(const double* x, int n) {
   double sum = 0.0;
   for (int i = 0; i < n; i++)
      sum += x[i] * x[i];
   return sqrt(sum);
}

int main(int argc, char* argv[]) {
   Version version;
   version();
   if (argc != 2) {
      std::cerr << argv[0] << ": invalid number of arguments\n";
      std::cerr << "Usage: " << argv[0] << "  no_of_elements\n";
      return 1;
   }
   int n = std::atoi(argv[1]); // number of elements in a 
   steady_clock::time_point ts, te;

   // allocate memory
   ts = steady_clock::now();
   double* a = new double[n];

   // populate vector a
   for (int i = 0; i < n; i++)
      a[i] = 1.0;
   te = steady_clock::now();
   reportTime(" - allocation and initialization", te - ts);

   // determine magnitude
   ts = steady_clock::now();
   double length = magnitude(a, n);
   te = steady_clock::now();
   reportTime(" - magnitude calculation", te - ts);

   // display result
   std::cout << "Magnitude of a[" << n << "] = " << length << std::endl;

   // deallocate host memory
   delete[] a;

   // terminate
   char c;
   std::cout << "Press Enter key to exit ... ";
   std::cin.get(c);
}