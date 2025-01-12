#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>
#include <atomic>

// Функція для обчислення часткового інтегралу
double calculate_integral_segment(int start, int end, int total_intervals) {
    double sum = 0.0;
    double step = 1.0 / total_intervals;
    for (int i = start; i < end; ++i) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum;
}

// Функція для виконання обчислень на кожному потоці
void compute_pi_segment(int start, int end, int total_intervals, std::atomic<double>& result) {
    double segment_sum = calculate_integral_segment(start, end, total_intervals);
    result.fetch_add(segment_sum, std::memory_order_relaxed);
}

int main() {
    const int num_intervals = 100000000;  // Кількість поділів
    const int num_threads = 4;            // Кількість потоків

    std::vector<std::thread> threads;
    std::atomic<double> pi(0.0); // Використовуємо атомарну змінну для безпечного доступу

    auto start_time = std::chrono::high_resolution_clock::now();

    int intervals_per_thread = num_intervals / num_threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * intervals_per_thread;
        int end = (t == num_threads - 1) ? num_intervals : (t + 1) * intervals_per_thread;
        threads.emplace_back(compute_pi_segment, start, end, num_intervals, std::ref(pi));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Остаточний результат
    pi /= num_intervals;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Liczba PI: " << pi.load() << std::endl;
    std::cout << "Czas: " << elapsed.count() << " sekund" << std::endl;

    return 0;
}
