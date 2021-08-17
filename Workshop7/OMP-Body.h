#pragma once
// Workshop 7 - Prefix Scan Comparison
// OMP-Body.h
// 2018.11.04
// Chris Szalwinski
#include <omp.h>

const int MAX_TILES = 8;

template <typename T, typename C>
T reduce(
   const T* in, // points to the data set
   int n,       // number of elements in the data set
   C combine,   // combine operation
   T initial    // initial value
) {

   for (int i = 0; i < n; i++)
      initial = combine(initial, in[i]);
   return initial;
}

template <typename T, typename C>
void incl_scan(
   const T* in,                // source data
   T* out,                     // output data
   int size,                   // size of data sets
   C combine,                  // combine operation
   T initial                   // initial value
) {

   for (int i = 0; i < size; i++) {
      initial = combine(initial, in[i]);
      out[i] = initial;
   }
}

template <typename T, typename C>
void excl_scan(
   const T* in,                // source data
   T* out,                     // output data
   int size,                   // size of data sets
   C combine,                  // combine operation
   T initial                   // initial value
) {

   if (size > 0) {
      for (int i = 0; i < size - 1; i++) {
         out[i] = initial;
         initial = combine(initial, in[i]);
      }
      out[size - 1] = initial;
   }
}

template <typename T, typename R, typename C, typename S>
void scan(
   const T* in,   // source data
   T* out,        // output data
   int size,      // size of source, output data sets
   R reduce,      // reduction expression
   C combine,     // combine expression
   S scan_fn,     // scan function (exclusive or inclusive)
   T initial      // initial value
)
{
   if (size > 0) {
      // allocate memory for maximum number of tiles
      T stage1Results[MAX_TILES];
      T stage2Results[MAX_TILES];
#pragma omp parallel num_threads(MAX_TILES)
      {
         int itile = omp_get_thread_num();
         int ntiles = omp_get_num_threads();
         int tile_size = (size - 1) / ntiles + 1;
         int last_tile = ntiles - 1;
         int last_tile_size = size - last_tile * tile_size;

         // step 1 - reduce each tile separately
         stage1Results[itile] = reduce(in + itile * tile_size,
            itile == last_tile ? last_tile_size : tile_size,
            combine, T(0));
#pragma omp barrier

         // step 2 - perform exclusive scan on stage1Results
         // store exclusive scan results in stage2Results[]
#pragma omp single
         excl_scan(stage1Results, stage2Results, ntiles,
            combine, T(0));

         // step 3 - scan each tile separately using stage2Results[] 
         scan_fn(in + itile * tile_size, out + itile * tile_size,
            itile == last_tile ? last_tile_size : tile_size,
            combine, stage2Results[itile]);
      }
   }
}