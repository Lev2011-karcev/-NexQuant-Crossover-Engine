#include "EMA.h"
#include <vector>
#include <cmath>
#include  "SMA.h"


/*double calculate_EMA(vector<double>& prices, int index, int period){
    double k = 2 / (period + 1.0);
    if (index == period-1){
        double sum = 0;
        for (int i = 0; i < period; i++){
            sum += prices[i];
        }
        return sum / period;
    } 

    double ema_yesterday = calculate_EMA(prices, index-1, period);
    double curr_price = prices[index];
    return (curr_price * k) + (ema_yesterday * (1.0-k));
}*/ // будет переполнение стэка 
// реализация с циклом for

double calculate_EMA(vector<double>& prices, int index, int period){
    double answ = 0;
    double k = 2.0 / (period + 1.0);
    double start = [&prices, period]() {
        double sum = 0.0;
        // Цикл берет первые элементы в количестве period - 1
        for (int i = 0; i < period ; ++i) {
            sum += prices[i];
        }
        return sum;
    }() / period;
    double last = start;
    if (index == period-1){
        return last;
    }
    for (int i = period; i <= index; i++){
        answ = last * (1.0-k) + (prices[i] * k);
        last = answ;
    }
    return answ;
}