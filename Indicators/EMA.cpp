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

double calculate_EMA(deque<double>& prices, int index, int period, double prev_ema) {
    if (prices.empty()) return 0.0;

    double k = 2.0 / (period + 1.0);
    double curr_price = prices[index];

    // Если это самый первый запуск для этой монеты и предыдщего EMA еще нет в базе
    if (prev_ema == 0.0) {
        // Считаем стартовую точку как обычное SMA по текущему куску истории
        double sum = 0.0;
        for (int i = 0; i < prices.size(); ++i) {
            sum += prices[i];
        }
        return sum / prices.size();
    }

    // Во все последующие разы просто считаем по одной формуле в одну строчку!
    return (curr_price * k) + (prev_ema * (1.0 - k));
}