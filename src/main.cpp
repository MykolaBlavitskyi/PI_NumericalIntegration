#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

// Funkcja do obliczania części całki
double calculate_partial_integral(int start, int end, int total_intervals) {
    double sum = 0.0;
    double step = 1.0 / total_intervals;
    for (int i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum;
}

int main() {
    int num_intervals = 100000000;  // Кількість поділів
    int num_threads = 4;           // Кількість потоків

    std::vector<std::thread> threads;
    std::vector<double> results(num_threads, 0.0);

    auto start_time = std::chrono::high_resolution_clock::now();

    int intervals_per_thread = num_intervals / num_threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * intervals_per_thread;
        int end = (t == num_threads - 1) ? num_intervals : (t + 1) * intervals_per_thread;
        threads.emplace_back([start, end, &results, t, num_intervals]() {
            results[t] = calculate_partial_integral(start, end, num_intervals);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double pi = 0.0;
    for (const auto& result : results) {
        pi += result;
    }
    pi *= (1.0 / num_intervals);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Liczba PI: " << pi << std::endl;
    std::cout << "Czas: " << elapsed.count() << " sekund" << std::endl;

    return 0;
}
