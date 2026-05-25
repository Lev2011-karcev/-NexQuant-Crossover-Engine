// CrossoverState.h
#pragma once
#include <vector>
#include <string>
#include "json.hpp"


struct CrossoverState {
    std::string name;
    std::vector<double> prices;
    double prev_sma = 0, prev_ema = 0;
    double curr_sma = 0, curr_ema = 0;
};

// ✅ Макрос пиши ПОСЛЕ объявления структуры, в глобальном неймспейсе
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CrossoverState, name, prices, prev_sma, prev_ema, curr_sma, curr_ema);