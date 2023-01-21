///
/// \file StopWatch.h
///
#pragma once
#include "StopWatchException.h"
#include <chrono>
#include <vector>
/// \namespace jimo
/// \brief The main namespace for all classes in this library.
/// \namespace timing
/// \brief The namespace for all classes related to timing, such as
/// StopWatch and Timer.
namespace jimo::timing
{
    /// \class StopWatch
    /// \brief A stopwatch class
    ///
    /// This class provides methods for timing activities and includes a lap
    /// timer.
    ///
    /// <A HREF="https://en.cppreference.com/w/cpp/chrono/steady_clock">
    /// std::chrono::steady_clock</A> is used internally in this class. 

    /// Here is a program that illustrates the use of the StopWatch class:
    /// \include StopWatch/StopWatch.cpp
    template<typename clock_t = std::chrono::steady_clock>
    requires std::chrono::is_clock_v<clock_t>
    class StopWatch
    {
        public:
            /// \name Constructors and Copy/Move Operators
            ///@{

            /// \brief The default StopWatch constructor
            StopWatch() : m_running(false), 
        m_startTime(clock_t::time_point::min()) {} ;
            /// \brief Copy constructor
            StopWatch(const StopWatch&) = delete;
            /// \brief Move constructor
            StopWatch(StopWatch&&) = delete;
            /// \brief Copy operator=
            StopWatch& operator =(const StopWatch&) = delete;
            /// \brief Move operator=
            StopWatch& operator =(StopWatch&&) = delete;
            ///@}
            /// \name Methods
           ///@{
            ///
            /// \brief Retrieve the std::chrono::duration between the calls to Start() and Stop().
            /// \returns The difference between the start and stop times. 
            /// \exception StopWatchException if GetDuration is called while stop watch is running.
            /// \exception StopWatchException if GetDuration is called before Start() and Stop() are called.
            std::chrono::nanoseconds getDuration() const
            {
                if (m_running)
                {
                    throw StopWatchException(
                        "Cannot retrieve duration from StopWatch that is currently running");
                }
                else if (m_startTime == clock_t::time_point::min())
                {
                    throw StopWatchException(
                        "Cannot retrieve duration from StopWatch that has not been run");
                }
                return *(m_laps.crbegin()) - m_startTime;
            }
            /// \brief Retrieve lap times
            /// \returns A vector of lap times.
            /// \exception StopWatchException if GetLapTimes() is called while the stop watch
            /// is running.
            /// \exception StopWatchException if GetLapTimes() called before stop watch ever started.
            std::vector<std::chrono::nanoseconds> getLapTimes()
            {
                if (m_running)
                {
                    throw StopWatchException("Cannot retrieve lap times while StopWatch is running");
                }
                else if (m_laps.size() == 0)
                {
                    throw StopWatchException("Cannot retrieve lap times. StopWatch never ran");
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
            /// \brief Start timing
            /// \exception StopWatchException if you call start() when the watch is already running.
            void start()
            {
                if (m_running)
                {
                    throw StopWatchException("Attempting to start a StopWatch that is already running!");
                }
                else
                {
                    m_running = true;
                    m_laps.clear();
                    m_startTime = clock_t::now();
                }
            }
            /// \brief Save timing of current lap and start timing of next lap
            /// \exception StopWatchException if you call startNextLap() when the watch is
            /// not running.
            void startNextLap()
            {
                if (m_running)
                {
                    m_laps.push_back(clock_t::now());
                }
                else
                {
                    throw StopWatchException("Cannot call startNextLap for stop watch that is not running.");
                }
            }
            /// \brief Stop timing
            /// \exception StopWatchException if you attempt to stop a stop watch that
            /// is not running.
            void stop()
            {
                if(m_running)
                {
                    m_laps.push_back(clock_t::now());
                    m_running = false;
                }
                else
                {
                    throw StopWatchException("Attempting to stop a StopWatch that is not running.");
                }
            }
            /// \brief Stop timing without saving the duration.
            /// \exception StopWatchException if call before stop watch is started.
            void stopWithoutSavingTime()
            {
            if(m_running)
            {
                m_running = false;
            }
            else
            {
                throw StopWatchException("stopWithoutSavingTime() being called before stop watch started");
            }
        }
            ///@}
        private:
            bool m_running;
            std::chrono::time_point<clock_t> m_startTime;
            std::vector<std::chrono::time_point<clock_t>> m_laps;
    };
}