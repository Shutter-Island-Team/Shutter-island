#include "./../include/FPSCounter.hpp"
#include <chrono>

FPSCounter::FPSCounter( double displayInterval )
    : m_refreshInterval(displayInterval > 0.0 ? Duration(displayInterval) : Duration(2.0)),
      m_lastRefreshTime(clock::now()), m_lastCallTime(clock::now()),
      m_accumulatedFPS(0.0f), m_fps(0.0f), m_numberOfSamples(0)
{}

FPSCounter::~FPSCounter()
{}

float FPSCounter::getFPS()
{
    TimePoint currentTime = clock::now();
    Duration duration = currentTime-m_lastCallTime;
    double currentFPS = double(1.0) / duration.count();
    m_accumulatedFPS += currentFPS;
    ++m_numberOfSamples;

    // It's time to update the value
    if( currentTime > m_lastRefreshTime + m_refreshInterval )
    {
        m_fps = m_accumulatedFPS / double(m_numberOfSamples);
        m_accumulatedFPS = double(0);
        m_numberOfSamples = 0;
        m_lastRefreshTime = currentTime;
    }

    m_lastCallTime = currentTime;
    return m_fps;
}
