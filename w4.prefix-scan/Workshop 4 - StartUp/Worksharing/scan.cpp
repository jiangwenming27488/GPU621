// Workshop 4 - Prefix Scan - Worksharing Three-Stage Tiled
// scan.cpp
// 2020.10.12
// Chris Szalwinski

#include <iostream>
#include <chrono>
#include "scan.h"

// report system time
//
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(span);
	std::cout << msg << " - took - " <<
		ms.count() << " microseconds" << std::endl;
}

int main(int argc, char** argv) {
	if (argc > 2) {
		std::cerr << argv[0] << ": invalid number of arguments\n";
		std::cerr << "Usage: " << argv[0] << "\n";
		std::cerr << "Usage: " << argv[0] << "  power_of_2\n";
		return 1;
	}
	std::cout << "Serial Prefix Scan" << std::endl;

	// initial values for testing
	const int N = 9;
	const int in_[N]{ 3, 1, 7, 0, 1, 4, 5, 9, 2 };

	// command line arguments - none for testing, 1 for large arrays
	int n, nt{ 1 };
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

	std::chrono::steady_clock::time_point ts, te;

	// Inclusive Prefix Scan - Remove Startup Cost
	scan(in, out, n, reduce<int, decltype(add)>, add, incl_scan<int, decltype(add)>, (int)0);

	// Inclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = scan(in, out, n, reduce<int, decltype(add)>, add, incl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();

	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 1] << std::endl;
	reportTime("Inclusive Scan", te - ts);

	// Exclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = scan(in, out, n, reduce<int, decltype(add)>, add, excl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();

	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	for (int i = 0; i < N; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 1] << std::endl;
	reportTime("Exclusive Scan", te - ts);

	// Exclusive Prefix Scan
	ts = std::chrono::steady_clock::now();
	nt = scan(in, out, n, reduce<int, decltype(add)>, add, incl_scan<int, decltype(add)>, (int)0);
	te = std::chrono::steady_clock::now();

	std::cout << nt << " thread" << (nt > 1 ? "s" : "") << std::endl;
	std::cout << 0 << ' ';
	for (int i = 0; i < N - 1; i++)
		std::cout << out[i] << ' ';
	std::cout << out[n - 2] << std::endl;
	reportTime("Exclusive Scan", te - ts);

	delete[] in;
	delete[] out;
}