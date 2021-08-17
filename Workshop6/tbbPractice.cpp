#include <iostream>
//#include <tbb/tbb.h>
//#include <tbb/parallel_reduce.h>

//int main() {
//   int a[] = { 1,2,3,4,5,6,7,8 };
//   int b[] = { 1,1,1,1,1,1,1,1 };
//   auto iaxpy = [&](int i) {
//      b[i] = 2 * a[i] + b[i];
//   };
//
//   tbb::parallel_for(0, 8, iaxpy);
//   for (int i = 0; i < 8; i++)
//      std::cout << b[i] << ' ';
//   std::cout << std::endl;
//}

//int main() {
//   int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
//   int b[] = {1, 1, 1, 1, 1, 1, 1, 1};
//
//   auto iaxpy = [&](tbb::blocked_range<int>& r) {
//      for (int i = r.begin(); i != r.end(); i++) {
//         b[i] = 2 * a[i] + b[i];
//      }
//   };
//
//   size_t grainsize = 100000;
//   //tbb::simple_partitioner();
//   tbb::blocked_range<int> range(0, 8, grainsize);
//   tbb::parallel_for(range, iaxpy);
//
//   for (int i = 0; i < 8; i++) {
//      std::cout << b[i] << ' ';
//   }
//   std::cout << std::endl;
//
//   return 0;
//}

// tbb::parallel_reduce(const Range&, const Value&, const Func&, const Reduce&);

//template<typename T, typename C>
//T reduce(const T* first, const T* last, T identity, C combine) {
//   return
//      tbb::parallel_reduce(
//         tbb::blocked_range<const T*>(first, last),
//         identity,
//         // operates on each chunk of the range
//         [&](tbb::blocked_range<const T*> r, T partial) -> T {
//            for (auto i = r.begin(); i != r.end(); i++)
//               partial = combine(*i, partial);
//            return partial;
//         },
//         // operates on the result on each chunk's reduction
//            combine
//         );
//
//}
//
//int main() {
//   int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
//   auto add = [](int a, int b) { return a + b; };
//   int sum = reduce(a, a + 8, 0, add);
//   std::cout << "sum = " << sum << std::endl;
//
//   return 0;
//}

//template<typename T, typename C>
//class Body {
//   T accumul;
//   T* const out;
//   const T* const in;
//   const T identity;
//   const C combine;
//public: 
//   Body(T* out_, const T* in_, T i, C c) : accumul(i), out(out_), in(in_), identity(i), combine(c) {}
//
//   T get_accumul() const { return accumul; }
//
//   // Functor
//   template<typename Tag>
//   void operator()(const tbb::blocked_range<int>& r, Tag) {
//      T temp = accumul;
//
//      for (int i = r.begin(); i != r.end(); i++) {
//         temp = combine(temp, in[i]);
//         if (Tag::is_final_scan())
//            out[i] = temp;
//      }
//
//      accumul = temp;
//   }
//
//   Body(Body& b, tbb::split) : accumul(b.identity), out(b.out), in(b.in), identity(b.identity), combine(b.combine) {}
//
//   void reverse_join(Body& a) {
//      accumul = combine(accumul, a.accumul);
//   }
//
//   void assign(Body& b) {
//      accumul = b.accumul;
//   }
//
//};
//
//
//template<typename T, typename C>
//T scan(T* out, const T* in, int n, T identity, C combine) {
//   Body<T, C> body(out, in, identity, combine);
//
//   tbb::parallel_scan(tbb::blocked_range<int>(0, n), body); // <- functor
//
//   return body.get_accumul();
//
//}
//
//int _main() {
//   int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
//   int b[8];
//   auto add = [](int a, int b) { return a + b; };
//   int sum = scan(b, a, 8, 0, add);
//   
//
//   for (int i = 0; i < 8; i++)
//      std::cout << b[i] << ' ';
//   std::cout << std::endl;
//
//   std::cout << "Sum = " << sum << std::endl;
//
//   return 0;
//}
//
//
