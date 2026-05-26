#ifndef EMA_H
#define EMA_H
#include <vector>
#include <deque>
using namespace std;

double calculate_EMA(deque<double>& prices, int index, int period, double prev_ema = 0.0);

#endif