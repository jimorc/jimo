#include "StopWatch.h"

namespace jimo::timing
{
    StopWatch::StopWatch() : m_running(false) {}

    std::chrono::nanoseconds StopWatch::Duration() const noexcept
    {
        return m_stopTime - m_startTime;
    }
 
    void StopWatch::Start()
    {
        if (m_running)
        {
            throw StopWatchException("Attempting to start a stop watch that is already running!");
        }
        else
        {
            m_running = true;
            m_startTime = std::chrono::steady_clock::now();
        }
    }

    void StopWatch::Stop()
    {
        if(m_running)
        {
            m_stopTime = std::chrono::steady_clock::now();
            m_running = false;
        }
        else
        {
            throw StopWatchException("Attempting to stop a StopWatch that is not running.");
        }
    }
}