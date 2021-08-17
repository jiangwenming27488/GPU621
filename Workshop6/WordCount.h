// Nathan Olah
// Student Number: 124723198

#pragma once
#include <iostream>
#include <tbb/tbb.h>

class WordCount {
   const char* str;
   int* const size;
   int* const numb;
   int len;
   bool(*delimiter)(char c);
public:
   WordCount(
      const char* str_, 
      int* const size_,
      int* const numb_,
      int len_,
      bool(*delimiter_)(char c)
   ) : str(str_), size(size_), numb(numb_), len(len_), delimiter(delimiter_) {}
   ~WordCount() {}

   // Blocked ranged
   void operator()(const tbb::blocked_range<int>& r) const {
      for (int i = r.begin(); i < r.end(); i++) {
         if (!delimiter(str[i])) {
            int s = 0;
            while (i + s < len && !delimiter(str[i + s])) s++;
            size[i] = s;
            int n = 0;
            for (int j = i + s + 1; j + s < len; j++) {
               bool bad = false;
               for (int k = 0;
                  k < s && k + i < len && k + j < len; k++) {
                  if (str[i + k] != str[j + k]) {
                     bad = true;
                     break;
                  }
               }
               if (!bad && delimiter(str[j + s])) n++;
            }
            numb[i] = n;
         }
         else {
            size[i] = 0;
            numb[i] = 0;
         }
         i += size[i];
      }
   }

   // Serial Functor 
   //void operator()(int s, int len) {
   //   for (int i = 0; i < len; i++) {
   //      if (!delimiter(str[i])) {
   //         int s = 0;
   //         while (i + s < len && !delimiter(str[i + s])) s++;
   //         size[i] = s;
   //         int n = 0;
   //         for (int j = i + s + 1; j + s < len; j++) {
   //            bool bad = false;
   //            for (int k = 0;
   //               k < s && k + i < len && k + j < len; k++) {
   //               if (str[i + k] != str[j + k]) {
   //                  bad = true;
   //                  break;
   //               }
   //            }
   //            if (!bad && delimiter(str[j + s])) n++;
   //         }
   //         numb[i] = n;
   //      }
   //      else {
   //         size[i] = 0;
   //         numb[i] = 0;
   //      }
   //      i += size[i];
   //   }
   //}
};
