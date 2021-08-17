// Workshop 4 - Prefix Scan
// w4.balanced-tree.cpp
// 2018.06.30
// Chris Szalwinski

#include <iostream>
#include <chrono>
#include <omp.h>

template <typename T, typename C>
int incl_scan(
	T* out,      // output data
	int size,    // size of source, output data sets
	C combine,   // combine expression
	T initial    // initial value
)
{
	int nt = 1;

	// upsweep (reduction)
	for (int stride = 1; stride < size; stride <<= 1) {
		{
#pragma omp parallel for
			for (int i = 0; i < size; i += 2 * stride)
				out[2 * stride + i - 1] = combine(out[2 * stride + i - 1],
					out[stride + i - 1]);
		}
	}

	// clear last element
	T last = out[size - 1];
	out[size - 1] = T(0);

	// downsweep
	for (int stride = size / 2; stride > 0; stride >>= 1) {
#pragma omp parallel for
		for (int i = 0; i < size; i += 2 * stride) {
			T temp = out[stride + i - 1];
			out[stride + i - 1] = out[2 * stride + i - 1];
			out[2 * stride + i - 1] = combine(temp, out[2 * stride + i - 1]);
		}
	}

	// shift left for inclusive scan and add last
#pragma omp parallel for
	for (int i = 0; i < size - 1; i++)
		out[i] = out[i + 1];
	out[size - 1] = last;

	return nt;
}

template <typename T, typename C>
int excl_scan(
	T* out,      // output data
	int size,    // size of source, output data sets
	C combine,   // combine expression
	T initial    // initial value
)
{
	int nt = 1;

	// upsweep (reduction)
	for (int stride = 1; stride < size; stride <<= 1) {
		{
#pragma omp parallel for
			for (int i = 0; i < size; i += 2 * stride)
				out[2 * stride + i - 1] = combine(out[2 * stride + i - 1],
					out[stride + i - 1]);
		}
	}

	// clear last element
	out[size - 1] = T(0);

	// downsweep
	for (int stride = size / 2; stride > 0; stride >>= 1) {
#pragma omp parallel for
		for (int i = 0; i < size; i += 2 * stride) {
			T temp = out[stride + i - 1];
			out[stride + i - 1] = out[2 * stride + i - 1];
			out[2 * stride + i - 1] = combine(temp, out[2 * stride + i - 1]);
		}
	}

	return nt;
}

template <typename T, typename C, typename S>
int scan(
	T* out,        // output data
	int size,      // size of source, output data sets
	C combine,     // combine expression
	S scan_fn,     // scan function (exclusive or inclusive) 
	T initial      // initial value
)
{
	return scan_fn(out, size, combine, T(0));
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
	std::cout << "Balanced Prefix Scan" << std::endl;

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
	int* outi = new int[n];
	int* oute = new int[n];

	// initialize
	for (int i = 0; i < N; i++)
		outi[i] = in_[i];
	for (int i = N; i < n; i++)
		outi[i] = 1;
	for (int i = 0; i < n; i++)
		oute[i] = outi[i];
	auto add = [](int a, int b) { return a + b; };

	// Inclusive Prefix Scan
	std::chrono::steady_clock::time_point ts, te;
	ts = std::chrono::steady_clock::now();
	nt = scan(outi, n, add, incl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();
	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << outi[i] << ' ';
	std::cout << outi[n - 1] << std::endl;
	reportTime("Inclusive Prefix Scan", te - ts);

	// Exclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = scan(oute, n, add, excl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();
	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << oute[i] << ' ';
	std::cout << oute[n - 1] << std::endl;
	reportTime("Exclusive Prefix Scan", te - ts);

	delete[] outi;
	delete[] oute;
}