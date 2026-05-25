#include <iostream>
#include <fstream>
#include "Indicators/EMA.h"
#include "Indicators/SMA.h"
#include "Structures/CrossoverState.h"
#include "Structures/Tick.h"
#include "json.hpp"
#include <unordered_map>
#include <cmath> // Для abs()

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
    
    while(true){
        Tick new_one;
        cout << "\nName: " ;
        cin >> new_one.symbol;
        cout << "price: ";
        cin >> new_one.price;
        
        int period_sma, period_EMA;
        cout << "SMA period: ";
        cin >> period_sma;
        cout << "EMA period: ";
        cin >> period_EMA;
        
        CrossoverState& state = db[new_one.symbol];
        
        state.name = new_one.symbol;
        state.prices.push_back(new_one.price);
        
        int size_price = max(period_sma, period_EMA);
        if (state.prices.size() > size_price){
            state.prices.erase(state.prices.begin());
        } else if (state.prices.size() < size_price){
            cout <<"Not Enough data " << "\n";
            
            // Даже если данных мало, сохраняем добавленную цену
            ofstream file_out(filename);
            json j_out = db;
            file_out << j_out.dump(4);
            file_out.close();
            continue;
        }
        
        state.prev_sma = state.curr_sma;
        state.prev_ema = state.curr_ema;
        
        double sma = SMA(state.prices, period_sma);
        int last_idx = state.prices.size() - 1;
        double ema = calculate_EMA(state.prices, last_idx, period_EMA, state.prev_ema);
        
        state.curr_sma = sma;
        state.curr_ema = ema;
        
        if (state.prev_sma == 0.0 || state.prev_ema == 0.0) {
            cout << " --- First calculation complete. Need one more tick for signals. ---\n";
            
            ofstream file_out(filename);
            json j_out = db;
            file_out << j_out.dump(4);
            file_out.close();
            continue; 
        }

        double diffPercent = abs(state.curr_ema - state.curr_sma) / state.curr_sma * 100.0;
        
        if (diffPercent > 2.0){
            // Проверяем условие BUY
            if (state.prev_ema <= state.prev_sma && state.curr_ema > state.curr_sma){
                if (state.pos != "BUY"){ // Если мы ЕЩЕ НЕ были в BUY — выводим надпись
                    state.pos = "BUY";
                    cout << " --------> BUY <--------" << "\n";
                }
            } 
            // Проверяем условие SELL
            else if (state.prev_ema >= state.prev_sma && state.curr_ema < state.curr_sma){
                if (state.pos != "SELL"){ // Если мы ЕЩЕ НЕ были в SELL — выводим надпись
                    state.pos = "SELL";
                    cout << " --------< SELL >--------" << "\n";
                }
            } 
            // Если тренд сильный, но пересечения прямо сейчас нет
            else {
                cout << " --------| TREND CONTINUES |--------" << "\n";
            }
        } else {
            // Если попали в зону флэта (меньше 2%)
            if (state.pos != "HOLD"){
                state.pos = "HOLD";
                cout << " --------| HOLD |--------" << "\n";
            }
        }
        
        // ЭТОТ БЛОК ТЕПЕРЬ СРАБАТЫВАЕТ ВСЕГДА!
        ofstream file_out(filename);
        json j_out = db;
        file_out << j_out.dump(4); 
        file_out.close();
    }
}
