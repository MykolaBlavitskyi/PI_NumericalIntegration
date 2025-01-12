#ifndef INTEGRAL_CALCULATOR_H
#define INTEGRAL_CALCULATOR_H

#include <vector>

// Funkcja do obliczania części całki
double calculate_partial_integral(int start, int end, int total_intervals);

// Funkcja do równoległego obliczania liczby PI
double calculate_pi(int total_intervals, int num_threads);

#endif // INTEGRAL_CALCULATOR_H
