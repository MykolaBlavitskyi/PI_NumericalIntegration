#include "integral_calculator.h"
#include <thread>
#include <vector>

double calculate_partial_integral(int start, int end, int total_intervals) {
    double sum = 0.0;
    double step = 1.0 / total_intervals;
    for (int i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum;
}

double calculate_pi(int total_intervals, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<double> results(num_threads, 0.0);

    int intervals_per_thread = total_intervals / num_threads;

    for (int t = 0; t < num_threads; ++t) {
        int start = t * intervals_per_thread;
        int end = (t == num_threads - 1) ? total_intervals : (t + 1) * intervals_per_thread;

        threads.emplace_back([start, end, &results, t, total_intervals]() {
            results[t] = calculate_partial_integral(start, end, total_intervals);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double pi = 0.0;
    for (const auto& result : results) {
        pi += result;
    }

    return pi * (1.0 / total_intervals);
}
