//// TBB - Hello World
//// tbb.cpp
//#include <iostream>
//#include <tbb/tbb.h>
//#include <tbb/parallel_reduce.h>
//
//// REDUCE
////template<typename T, typename C>
////T reduce(const T* first, const T* last, T identity, C combine) {
////   return tbb::parallel_reduce(
////      tbb::blocked_range<const T*>(first, last),
////      identity,
////      [&](tbb::blocked_range<const T*> r, T partial) -> T { 
////         for (auto x = r.begin(); x != r.end(); x++) 
////            partial = combine(*x, partial); 
////         return partial; 
////      },
////      combine);
////
////}
////
////int main() {
////   int a[] = {1,2,3,4,5,6,7,8};
////   auto add = [](int a, int b) { return a + b; };
////   int sum = reduce(a, a + 8, 0, add);
////   std::cout << "Sum : " << sum << std::endl;
////
////}
//
////SCAN
//template<typename T, typename C>
//class Body {
//   T accumul;
//   T* const out;
//   const T* const in;
//   const T identity;
//   const C combine;
//public:
//   Body(T* out_, const T* in_, T i, C c) : accumul(i), out(out_), in(in_),
//      identity(i), combine(c) {}
//
//   T get_accumul() const { 
//      return accumul;
//   }
//
//   template<typename Tag>
//   void operator()(const tbb::blocked_range<int>& r, Tag) {
//      T temp = accumul;
//      for (int i = r.begin(); i != r.end(); i++) {
//         temp = combine(temp, in[i]);
//
//         if (Tag::is_final_scan())
//            out[i] = temp;
//
//      }
//      accumul = temp;
//   }
//
//   Body(Body& b, tbb::split) : accumul(b.identity), out(b.out), in(b.in),
//      identity(b.identity), combine(b.combine) {}
//
//   void reverse_join(Body& a) {
//      accumul = combine(accumul, a.accumul);
//   }
//
//   void assign(Body& b) { 
//      accumul = b.accumul;
//   }
//};
//
//
//
//template<typename T, typename C>
//T scan(T* out, const T* in, int n, T identity, C combine) {
//   Body<T, C> body(out, in, identity, combine);
//
//   tbb::parallel_scan(tbb::blocked_range<int>(0, n), body);
//
//   return body.get_accumul();
//}
//
//int main() {
//   int a[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//   int b[8];
//   auto add = [](int a, int b) { return a + b; };
//   int sum = scan(b, a, 8, 0, add);
//   for (int i = 0; i < 8; i++)
//      std::cout << b[i] << " ";
//   std::cout << std::endl;
//   std::cout << "Sum = " << sum << std::endl;
//}

#include <iostream>
#include <cctype>
#include <tbb/tbb.h>
int main() {
   char str[]{ "The Standard Template Library" };
   auto func = [&](tbb::blocked_range<char>& r) {
      for (int i = r.begin(); i != r.end(); i++) {
         if (str[i] < 'R' && str[i] > 'C')
            str[i] = std::tolower(str[i]);
      }
   };

   tbb::blocked_range<char> range(0, 30);
   tbb::parallel_for(range, func);
   std::cout << str << std::endl;
}

//#include <iostream>
//#include <cctype>
//int main() {
//   char str[]{ "The Standard Template Library" };
//   for (int i = 0; str[i] != '\0'; ++i)
//      if (str[i] < 'R' && str[i] > 'C')
//         str[i] = std::tolower(str[i]);
//   std::cout << str << std::endl;
//}