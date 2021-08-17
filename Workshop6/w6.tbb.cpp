// Workshop 6 - Word Count Algorithm
// w6.tbb.cpp
// Chris Szalwinski
// 2018/10/28

#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <tbb/tbb.h>
#include "WordCount.h"
using namespace std::chrono;

// report system time
//
void reportTime(const char* msg, steady_clock::duration span) {
   auto ms = duration_cast<milliseconds>(span);
   std::cout << msg << " - took - " <<
      ms.count() << " milliseconds" << std::endl;
}

int main(int argc, char** argv) {
   if (argc == 1 || argc > 3) {
      std::cerr << "*** Incorrect number of arguments ***\n";
      std::cerr << "Usage: " << argv[0] << " filename [grainsize]\n";
      return 1;
   }
   unsigned grainsize = 1;
   if (argc == 3) grainsize = (unsigned)atoi(argv[2]);

   // load text from file to string
   std::ifstream file(argv[1]);
   if (!file) {
      std::cerr << "*** Incorrect file name ***\n";
      std::cerr << "Filename " << argv[1] << " does not exist\n";
      return 2;
   }
   std::string str;
   file.seekg(0, std::ios::end);
   // request a change in capacity
   str.reserve((unsigned)file.tellg());
   file.seekg(0, std::ios::beg);
   str.assign((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());

   // allocate memory for analysis
   steady_clock::time_point ts, te;
   int len = (int)str.size();
   int* size = new int[len];
   int* numb = new int[len];

   // define delimiters
   auto delimiter = [](char s) {
      return s == ' ' || s == '.' || s == ',' || s == '\n'; };

   ts = steady_clock::now();
   WordCount wordcount(str.c_str(), size, numb, len, delimiter);
   // replace: wordcount(0, len);
   // with tbb statements
   tbb::simple_partitioner();
   tbb::parallel_for(tbb::blocked_range<int>(0, len, grainsize), wordcount);
   te = steady_clock::now();

   // remove duplicate words
   for (int i = 0; i < len; i++) {
      if (size[i]) {
         for (int j = i + 1; j < len; j++) {
            if (size[j] == size[i]) {
               bool duplicate = true;
               for (int k = 0; k < size[i]; k++) {
                  if (str[i + k] != str[j + k]) {
                     duplicate = false;
                     break;
                  }
               }
               if (duplicate) {
                  numb[j] = 0;
               }
            }
         }
      }
   }

   // determine longest word
   int maxsize = 0;
   int maxpos = 0;
   for (int i = 0; i < len; ++i) {
      if (size[i]) {
         if (numb[i] != 0) {
            std::cout << numb[i] << " * ";
            for (int j = 0; j < size[i]; ++j)
               std::cout << str[i + j];
            std::cout << std::endl;
            if (size[i] > maxsize) {
               maxsize = size[i];
               maxpos = i;
            }
         }
         i += size[i];
      }
   }

   // report the longest word
   if (maxsize) {
      std::cout << "\nLongest Word\n";
      std::cout << numb[maxpos] << " * ";
      for (int k = 0; k < maxsize; k++)
         std::cout << str[maxpos + k];
      std::cout << std::endl;
   }
   else {
      std::cout << "no word repetitions found" << std::endl;
   }
   reportTime(argv[1], te - ts);

   // deallocate
   delete[] size;
   delete[] numb;
   std::cout << "TBB Solution" << std::endl;
}