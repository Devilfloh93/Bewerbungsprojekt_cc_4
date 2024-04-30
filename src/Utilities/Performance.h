#pragma once
#include "Game.h"
#include "nlohmann/json.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using json = nlohmann::json;
using namespace std;
using ClockType = chrono::steady_clock;

class Performance
{

public:
    Performance();
    ~Performance() = default;

    void TestBegin();
    void TestEnd();

private:
    ClockType::time_point m_startTime;
    ClockType::time_point m_endTime;
};
