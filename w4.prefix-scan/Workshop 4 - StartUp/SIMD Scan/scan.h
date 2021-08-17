// Workshop 4 - Prefix Scan - SIMD Scan
// scan.h
#include <omp.h>

template <typename T>
int incl_scan(
    const T* in,                // source data
    T* out,                     // output data
    int size,                   // size of data sets
    T initial                   // initial value
) {
#pragma omp simd reduction(inscan, +:initial)
    for (int i = 0; i < size; i++) {
        initial += in[i];
#pragma omp scan inclusive(initial)
        out[i] = initial;
    }
    return 1; // 1 thread
}

template <typename T>
int excl_scan(
    const T* in,                // source data
    T* out,                     // output data
    int size,                   // size of data sets
    T initial                   // initial value
) {

    if (size > 0) {
#pragma omp simd reduction(inscan, +:initial)
        for (int i = 0; i < size - 1; i++) {
            out[i] = initial;
#pragma omp scan exclusive(initial)
            initial += in[i];
        }
        out[size - 1] = initial;
    }
    return 1; // 1 thread
}

template <typename T, typename S>
int scan(
    const T* in,   // source data
    T* out,        // output data
    int size,      // size of source, output data sets
    S scan_fn,     // scan function (exclusive or inclusive) 
    T initial      // initial value
)
{
    return scan_fn(in, out, size, T(0));
}
