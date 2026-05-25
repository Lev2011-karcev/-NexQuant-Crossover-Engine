# 📈 NexQuant Crossover Engine (NCE)

```text
  _   _           ___                      _     _____             _             

 | \ | |         / _ \                    | |   |  ___|           (_)            
 |  \| | ___  __| / \ |_   _  __ _ _ __   | |_  | |__  _ __   __ _ _ _ __   ___  
 | . ` |/ _ \/ _` | | | | | |/ _` | '_ \  | __| |  __|| '_ \ / _` | | '_ \ / _ \ 
 | |\  |  __/ (_| \ \_/ / |_| | (_| | | | | |_  | |___| | | | (_| | | | | |  __/ 
 |_| \_|\___|\__,_|\___/ \__,_|\__,_|_| |_|  __| \____/_| |_|\__, |_|_| |_|\___| 
                                                              __/ |              

                                                             |___/               
```

**NexQuant Crossover Engine** is a lightweight, high-performance, multi-asset event-driven trading logic prototype written in modern C++. It tracks real-time market data ticks, calculates Simple Moving Average (SMA) and Exponential Moving Average (EMA) indicators on the fly, and generates precise `BUY`, `SELL`, or `HOLD` signals based on a 2% trend-breakout filter.

---

## ⚡ Key Features

* **Multi-Asset Architecture:** Uses `std::unordered_map` to store and access individual asset states instantaneously with \(O(1)\) time complexity.
* **Persistent State Memory:** Integrates `nlohmann/json` to save the complete session state (prices history, positions, and intermediate steps) to a local file. The engine resumes exactly where it left off after a restart.
* **Optimized Math Pipeline:** Implements an iterative, memory-efficient EMA calculation that bypasses recursion to eliminate `stack overflow` risks and avoids array fragmentation.
* **Market Noise Protection:** Features a strict `2% threshold filter` combined with an intelligent *Trend Breakout* tracker to filter out flat markets, whip-saws, and false crossover signals.

---

## 📂 Project Structure

```text
📂 NexQuant-Engine/
 ├── 📄 Crossover.cpp             # Main runtime execution loop (Engine Core)
 ├── 📄 json.hpp                  # Modern JSON header-only library
 ├── 📄 README.md                 # Project documentation
 ├── 📂 Indicators/
 │    ├── 📄 SMA.h / SMA.cpp      # Simple Moving Average calculation logic
 │    └── 📄 EMA.h / EMA.cpp      # Non-recursive Exponential Moving Average logic
 └── 📂 Structures/
      ├── 📄 Tick.h               # Inbound market data model
      └── 📄 CrossoverState.h     # Internal persistent database schema
```

---

## 🛠 Compilation and Setup

Since this is a multi-file project, you need to compile all translation units (`.cpp` files) together and tell the compiler where to look for headers using the `-I .` flag.

Open your terminal in the project root directory and run:

```bash
clang++ Crossover.cpp Indicators/SMA.cpp Indicators/EMA.cpp -I . -o crossover
```

---

## 🚀 How to Run

Execute the compiled binary from your terminal:

```bash
./crossover
```

---

## 🧪 Step-by-Step Test Case (What to Input)

To verify the signal logic and the 2% noise filter, use **SMA period: 3** and **EMA period: 3**. Enter the following values sequentially to simulate a real market cycle:

### Phase 1: Accumulating Price History
1. **Name:** `AAPL` | **price:** `100` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: Not Enough data (1/3)*
2. **Name:** `AAPL` | **price:** `100` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: Not Enough data (2/3)*
3. **Name:** `AAPL` | **price:** `100` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: First calculation complete. Need one more tick for signals.*

### Phase 2: Generating Triggers
4. **Name:** `AAPL` | **price:** `110` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: `--------| HOLD (Weak Cross Up) |--------` (Lines separated, but under 2% threshold)*
5. **Name:** `AAPL` | **price:** `130` | **SMA period:** `3` | **EMA period:** `3` ➔ **Output: `--------> BUY (Trend Breakout) <--------`** *(Explosive growth confirmed!)*
6. **Name:** `AAPL` | **price:** `135` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: `--------| TREND CONTINUES |--------` (Riding the trend upward)*
7. **Name:** `AAPL` | **price:** `120` | **SMA period:** `3` | **EMA period:** `3` ➔ *Output: `--------| HOLD |--------` (Market cool-down, lines converging)*
8. **Name:** `AAPL` | **price:** `80`  | **SMA period:** `3` | **EMA period:** `3` ➔ **Output: `--------< SELL (Trend Breakout) >--------`** *(Market crash detected, exiting position)*

---

## 💾 Database Output Example (`states.json`)

Data is securely dumped into the file system in an elegant, indented JSON structure after every single tick:

```json
{
    "AAPL": {
        "curr_ema": 111.25,
        "curr_sma": 110.0,
        "name": "AAPL",
        "pos": "BUY",
        "prev_ema": 100.0,
        "prev_sma": 100.0,
        "prices": [
            100.0,
            100.0,
            100.0,
            110.0,
            130.0
        ]
    }
}
```
