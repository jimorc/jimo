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

    class StopWatch
    {
        public:
            /// \name Constructors and Copy/Move Operators
            ///@{

            /// \brief The default StopWatch constructor
            StopWatch();
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
            std::chrono::nanoseconds GetDuration() const;
            /// \brief Retrieve lap times
            /// \returns A vector of lap times.
            /// \exception StopWatchException if GetLapTimes() is called while the stop watch
            /// is running.
            /// \exception StopWatchException if GetLapTimes() called before stop watch ever started.
                std::vector<std::chrono::nanoseconds> GetLapTimes();
            /// \brief Start timing
            /// \exception StopWatchException if you call Start() when the watch is already running.
            void Start();
            /// \brief Save timing of current lap and start timing of next lap
            /// \exception StopWatchException if you call StartNextLap() when the watch is
            /// not running.
            void StartNextLap();
            /// \brief Stop timing
            /// \exception StopWatchException if you attempt to stop a stop watch that
            /// is not running.
            void Stop();
            /// \brief Stop timing without saving the duration.
            /// \exception StopWatchException if call before stop watch is started.
            void StopWithoutSavingTime();
            ///@}
        private:
            bool m_running;
            std::chrono::time_point<std::chrono::steady_clock> m_startTime;
            std::vector<std::chrono::time_point<std::chrono::steady_clock>> m_laps;
    };
}