#include "SMA.h"



double SMA(vector<double>& prices,int period){
    double sum = 0;
    int start = prices.size() - period;
    for (int i = start; i < prices.size(); i++){ // index должке быть прямо номером а не индексом
        sum += prices[i];
    }
    return sum / period;
}