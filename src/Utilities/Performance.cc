#include "Performance.h"
#include <SFML/Graphics.hpp>

using ClockRes = chrono::microseconds;

Performance::Performance()
{
}

void Performance::TestBegin()
{
    m_startTime = ClockType::now();
}

void Performance::TestEnd()
{
    m_endTime = ClockType::now();
    const auto elapsed_time = chrono::duration_cast<ClockRes>(m_endTime - m_startTime).count();
    cout << "Elapsed time: " << elapsed_time << '\n';
}
