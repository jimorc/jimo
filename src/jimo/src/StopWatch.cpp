#include "StopWatch.h"
#import <algorithm>

namespace jimo::timing
{
    StopWatch::StopWatch() : m_running(false), 
        m_startTime(std::chrono::steady_clock::time_point::min()) {} 

    std::chrono::nanoseconds StopWatch::GetDuration() const
    {
        if (m_running)
        {
            throw StopWatchException(
                "Cannot retrieve duration from StopWatch that is currently running");
        }
        else if (m_startTime == std::chrono::steady_clock::time_point::min())
        {
            throw StopWatchException(
                "Cannot retrieve duration from StopWatch that has not been run");
        }
        return *(m_laps.crbegin()) - m_startTime;
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
            m_laps.clear();
            m_startTime = std::chrono::steady_clock::now();
        }
    }

    void StopWatch::Stop()
    {
        if(m_running)
        {
            m_laps.push_back(std::chrono::steady_clock::now());
            m_running = false;
        }
        else
        {
            throw StopWatchException("Attempting to stop a StopWatch that is not running.");
        }
    }

    void StopWatch::StartNextLap()
    {
        if (m_running)
        {
            m_laps.push_back(std::chrono::steady_clock::now());
        }
        else
        {
            throw StopWatchException("Cannot call StartNextLap for stop watch that is not running.");
        }
    }

    std::vector<std::chrono::nanoseconds> StopWatch::GetLapTimes()
    {
        if (m_running)
        {
            throw StopWatchException("Cannot retrieve lap times while stop watch is running");
        }
        else if (m_laps.size() == 0)
        {
            throw StopWatchException("Cannot retrieve lap times. Stop watch never ran");
        }
        else{
            std::vector<std::chrono::nanoseconds> lapsedTimes;
            auto lapIter = m_laps.cbegin();
            lapsedTimes.push_back(*lapIter - m_startTime);
            auto nextIter = lapIter + 1;
            while (nextIter < m_laps.cend())
            {
                lapsedTimes.push_back(*nextIter - *lapIter);
                ++lapIter;
                ++nextIter;
            }
            return lapsedTimes;
        }
    }
}