// Map Pattern
// saxpy_work_sharing.cpp

#include <iostream>
#include <chrono>
#include <omp.h>

void reportTime(const char* msg, std::chrono::steady_clock::duration span);

constexpr int N = 100000000;

void saxpy(float a, const float* x, float* y, int n) {
#pragma omp parallel for schedule(guided, 4)
//   {
//#pragma omp 
    for (int i = 0; i < n; ++i)
        y[i] = a * x[i] + y[i];
//    }
}

int main() {
    float* x = new float[N];
    float* y = new float[N];
    float a = 2.0f;

    // initialize
    for (int i = 0; i < N; i++)
        x[i] = y[i] = 1.0f;

    std::chrono::steady_clock::time_point ts, te;
    ts = std::chrono::steady_clock::now();
    saxpy(a, x, y, N);
    te = std::chrono::steady_clock::now();

    // sum y[i]
    float s = 0.0f;
    for (int i = 0; i < N; i++)
        s += y[i];

    std::cout << "Sum y[i] = " << s << std::endl;
    reportTime("saxpy", te - ts);

    delete[] x;
    delete[] y;
}

// reportTime inserts the duration (span) into standard output
//
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(span);
    std::cout << msg << ": " <<
        ms.count() << " microseconds" << std::endl;
}