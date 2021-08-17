// Workshop 4 - Prefix Scan - SPMD Three-Stage Tiled
// scan.h
#include <omp.h>

template <typename T, typename C>
void incl_scan(
	const T* in, // source data
	T* out,      // output data
	int size,    // size of source, output data sets
	C combine,   // combine expression
	T initial    // initial value
)
{
	for (int i = 0; i < size; i++) {
		initial = combine(initial, in[i]);
		out[i] = initial;
	}
}

template <typename T, typename C>
void excl_scan(
	const T* in, // source data
	T* out,      // output data
	int size,    // size of source, output data sets
	C combine,   // combine expression
	T initial    // initial value
)
{
	if (size > 0) {
		for (int i = 0; i < size - 1; i++) {
			out[i] = initial;
			initial = combine(initial, in[i]);
		}
		out[size - 1] = initial;
	}
}

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

template <typename T, typename R, typename C, typename S>
int scan(
	const T* in,   // source data
	T* out,        // output data
	int size,      // size of source, output data sets
	R reduce,      // reduction expression
	C combine,     // combine expression
	S scan_fn,     // scan function (exclusive or inclusive)
	T initial      // initial value
)
{
	int mtiles = 1;
	if (size > 0) {
		mtiles = omp_get_max_threads();
		// requested number of tiles
		T* reduced = new T[mtiles];
		T* scanRes = new T[mtiles];
		omp_set_num_threads(mtiles);

#pragma omp parallel
		{
			int itile = omp_get_thread_num();
			int ntiles = omp_get_num_threads();
			if (itile == 0) mtiles = ntiles;
			int tile_size = (size - 1) / ntiles + 1;

			//int tile_size = (size - 1) / mtiles + 1;
			int last_tile = mtiles - 1;
			int last_tile_size = size - last_tile * tile_size;

			tile_size = itile == ntiles - 1 ? last_tile_size : tile_size;
			// step 1 - reduce each tile separately
#pragma omp single
			{
				for (int itile = 0; itile < mtiles; ++itile)
					reduced[itile] = reduce(in + itile * tile_size,
						itile == last_tile ? last_tile_size : tile_size, combine, T(0));
			}

			// step 2 - perform exclusive scan on all tiles using reduction outputs 
			// store results in scanRes[]
			excl_scan(reduced, scanRes, mtiles, combine, T(0));
#pragma omp barrier
			

			// step 3 - scan each tile separately using scanRes[]
#pragma omp single
			{
				for (int itile = 0; itile < mtiles; ++itile)
					scan_fn(in + itile * tile_size, out + itile * tile_size,
						itile == last_tile ? last_tile_size : tile_size, combine,
						scanRes[itile]);
			}
			
		}

		delete[] reduced;
		delete[] scanRes;
	}
	return mtiles;
}
