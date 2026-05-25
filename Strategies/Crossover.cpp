#include <iostream>
#include <fstream>
#include "Indicators/EMA.h"
#include "Indicators/SMA.h"
#include "Structures/CrossoverState.h"
#include "Structures/Tick.h"
#include "json.hpp"
#include <unordered_map>

using json = nlohmann::json;


using namespace std;

int main(){
    string filename = "States/states.json";
    unordered_map<string, CrossoverState> db;
    ifstream file_in(filename);
    if (file_in.is_open() && file_in.peek() != ifstream::traits_type::eof()){
        json j_in;
        file_in >> j_in;
        db = j_in.get<unordered_map<string, CrossoverState>>();
        file_in.close();
    }
    Tick new_one;
    cout << "Name: " ;
    cin >> new_one.symbol;
    cout << endl << "price: ";
    cin >> new_one.price;
    int period_sma, period_EMA;
    cout << "SMA period: ";
    cin >> period_sma;
    cout << endl << "EMA period: ";
    cin >> period_EMA;
    db[new_one.symbol].name = new_one.symbol;
    db[new_one.symbol].prices.push_back(new_one.price);
    int size_price = max(period_sma, period_EMA);
    if (db[new_one.symbol].prices.size() > size_price){
        db[new_one.symbol].prices.erase(db[new_one.symbol].prices.begin());
    }else if (db[new_one.symbol].prices.size() < size_price){
        cout <<"Not Enough data " << "\n";
        return 0;
    }
    double sma = SMA(db[new_one.symbol].prices, period_sma);
    int last_idx = db[new_one.symbol].prices.size() - 1;
    double ema = calculate_EMA(db[new_one.symbol].prices, last_idx, period_EMA);
    db[new_one.symbol].curr_sma = sma;
    db[new_one.symbol].curr_ema = ema;
    CrossoverState& state = db[new_one.symbol];
    // написать логику пересечения сигналов 










    state.prev_sma = state.curr_sma;
    state.prev_ema = state.curr_ema;
    ofstream file_out(filename);
    json j_out = db;
    file_out << j_out;
    file_out.close();
}