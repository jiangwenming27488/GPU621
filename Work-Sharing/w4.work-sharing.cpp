// Workshop 4 - Prefix Scan
// w4.work-sharing.cpp
// 2018.06.30
// Chris Szalwinski

#include <iostream>
#include <chrono>
#include <omp.h>

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
	const int tile_size = 4;
	if (size > 0) {
		// calculate number of tiles = number of threads
		mtiles = omp_get_max_threads();
		// allocate memory for maximum number of tiles
		T* stage1Results = new T[mtiles];
		T* stage2Results = new T[mtiles];

#pragma omp parallel
		{
			// step 1 - reduce each tile separately
			int itile = 0;
			int ntiles = mtiles;
			//stage1Results[itile] = reduce(in, size, combine, T(0));

			int last_tile = ntiles - 1;
			int last_tile_size = size - last_tile * tile_size;

#pragma omp for
			for (itile = 0; itile < ntiles; itile++) {
				//stage1Results[itile] = reduce(in, size, combine, T(0));
				stage1Results[itile] = reduce(in + itile * mtiles, itile == last_tile ? last_tile_size : mtiles, combine, T(0));
			}

			// step 2 - perform exclusive scan on stage1Results 
			// store exclusive scan results in stage2Results[]
#pragma omp single
			{
				excl_scan(stage1Results, stage2Results, ntiles, combine, T(0));
			}

			// step 3 - scan each tile separately using stage2Results[]
			//scan_fn(in, out, size, combine, stage2Results[itile]);
#pragma omp for
			for (int itile = 0; itile < ntiles; itile++) {
				//scan_fn(in, out, size, combine, stage2Results[itile]);
				scan_fn(in + itile * mtiles, out + itile * mtiles, itile == last_tile ? last_tile_size : mtiles, combine, stage2Results[itile]);
			}


		}

		// deallocate memory
		delete[] stage1Results;
		delete[] stage2Results;
	}

	return mtiles;
}

// report system time
//
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(span);
	std::cout << msg << " - took - " <<
		ms.count() << " milliseconds" << std::endl;
}

int main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << argv[0] << ": invalid number of arguments\n";
		std::cerr << "Usage: " << argv[0] << "\n";
		std::cerr << "Usage: " << argv[0] << "  power_of_2\n";
		return 1;
	}
	std::cout << "Work Sharing Prefix Scan" << std::endl;

	// initial values for testing
	const int N = 9;
	const int in_[N]{ 3, 1, 7, 0, 1, 4, 5, 9, 2 };

	// command line arguments - none for testing, 1 for large arrays
	int n, nt;
	if (argc == 1) {
		n = N;
	}
	else {
		n = 1 << std::atoi(argv[1]);
		if (n < N) n = N;
	}
	int* in = new int[n];
	int* out = new int[n];

	// initialize
	for (int i = 0; i < N; i++)
		in[i] = in_[i];
	for (int i = N; i < n; i++)
		in[i] = 1;
	auto add = [](int a, int b) { return a + b; };

	// Inclusive Prefix Scan
	std::chrono::steady_clock::time_point ts, te;
	ts = std::chrono::steady_clock::now();
	nt = scan(in, out, n, reduce<int, decltype(add)>,
		add, incl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();
	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 1] << std::endl;
	reportTime("Inclusive Prefix Scan", te - ts);

	// Exclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = scan(in, out, n, reduce<int, decltype(add)>,
		add, excl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();
	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 1] << std::endl;
	reportTime("Exclusive Prefix Scan", te - ts);

	delete[] in;
	delete[] out;
}