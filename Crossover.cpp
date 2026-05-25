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
        state.prices.push_back(new_one.price); // Цена просто добавляется в хвост истории
        
        // ИСПРАВЛЕНИЕ: Мы БОЛЬШЕ НЕ УДАЛЯЕМ старые цены через erase!
        // Мы просто проверяем, накопилось ли достаточно данных для расчета.
        int required_size = max(period_sma, period_EMA);
        
        if (state.prices.size() < static_cast<size_t>(required_size)){
            cout <<"Not Enough data (" << state.prices.size() << "/" << required_size << ")\n";
            
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

        // 1. Считаем процент разницы между свежими индикаторами
        double diffPercent = abs(state.curr_ema - state.curr_sma) / state.curr_sma * 100.0;
        
        // 2. Определяем факт физического пересечения линий на этом тике
        bool is_cross_up = (state.prev_ema <= state.prev_sma && state.curr_ema > state.curr_sma);
        bool is_cross_down = (state.prev_ema >= state.prev_sma && state.curr_ema < state.curr_sma);

        if (is_cross_up) {
            // Линии пересеклись вверх — проверяем силу импульса
            if (diffPercent > 2.0) {
                state.pos = "BUY";
                cout << " --------> BUY <--------" << "\n";
            } else {
                state.pos = "HOLD";
                cout << " --------| HOLD (Weak Cross Up) |--------" << "\n";
            }
        } 
        else if (is_cross_down) {
            // Линии пересеклись вниз — проверяем силу импульса
            if (diffPercent > 2.0) {
                state.pos = "SELL";
                cout << " --------< SELL >--------" << "\n";
            } else {
                state.pos = "HOLD";
                cout << " --------| HOLD (Weak Cross Down) |--------" << "\n";
            }
        } 
        else {
            // Физического пересечения на этом тике не было
            if (diffPercent > 2.0) {
                // Если мы уже находимся в правильной позиции — подтверждаем тренд
                if ((state.curr_ema > state.curr_sma && state.pos == "BUY") || 
                    (state.curr_ema < state.curr_sma && state.pos == "SELL")) {
                    cout << " --------| TREND CONTINUES |--------" << "\n";
                } else {
                    // Если линии разошлись сильнее 2%, но пересечение случилось раньше (наш баг)
                    if (state.curr_ema > state.curr_sma && state.pos != "BUY") {
                        state.pos = "BUY";
                        cout << " --------> BUY (Trend Breakout) <--------" << "\n";
                    } else if (state.curr_ema < state.curr_sma && state.pos != "SELL") {
                        state.pos = "SELL";
                        cout << " --------< SELL (Trend Breakout) >--------" << "\n";
                    }
                }
            } else {
                // Разрыв маленький, пересечений нет — пилим во флэте
                if (state.pos != "HOLD") {
                    state.pos = "HOLD";
                    cout << " --------| HOLD |--------" << "\n";
                }
            }
        }
        
        ofstream file_out(filename);
        json j_out = db;
        file_out << j_out.dump(4); 
        file_out.close();
    }
}
