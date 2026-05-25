#ifndef CROSSOVERSTATE_H
#define CROSSOVERSTATE_H

#include <string>
#include <vector>
#include "../json.hpp"

struct CrossoverState{
    std::string name;
    std::vector<double> prices;
    double prev_sma = 0.0;
    double prev_ema = 0.0;
    double curr_sma = 0.0;
    double curr_ema = 0.0;
    std::string pos = "NONE";
};// если prev_ema <= prev_sma а стало curr_ema > curr_sma -> BUY
// если prev_ema >= prev_sma а стало curr_ema < curr_sma -> SELL
// если |curr_ema - curr_sma| 
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CrossoverState, name, prices, prev_sma, prev_ema, curr_sma,curr_ema);
#endif
/*NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CrossoverState, name, prices, prev_sma, prev_ema, curr_sma, curr_ema);*/