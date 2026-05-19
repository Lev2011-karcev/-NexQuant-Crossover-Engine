#include "SMA.h"



double SMA(vector<double>& prices,int index){
    double sum = 0;
    for (int i = 0; i < index; i++){ // index должке быть прямо номером а не индексом
        sum += prices[i];
    }
    return sum /index;
}