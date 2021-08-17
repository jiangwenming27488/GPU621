// Workshop 4 - Prefix Scan - Serial
// scan.h
// after McCool etal. (2012)

template <typename T, typename C>
int incl_scan(
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
    return 1; // 1 thread
}

template <typename T, typename C>
int excl_scan(
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
    return 1; // 1 thread
}

template <typename T, typename C, typename S>
int scan(
    const T* in,   // source data
    T* out,        // output data
    int size,      // size of source, output data sets
    C combine,     // combine expression
    S scan_fn,     // scan function (exclusive or inclusive) 
    T initial      // initial value
)
{
    return scan_fn(in, out, size, combine, T(0));
}
