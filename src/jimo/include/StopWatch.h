///
/// \file StopWatch.h
///
#pragma once
#include "StopWatchException.h"
#import <chrono>
#import <vector>
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
    /// Using <A HREF="https://en.cppreference.com/w/cpp/chrono/system_clock">
    /// std::chrono::system_class</A> would cause problems around time change
    /// times (e.g. when switching from daylight savings time to standard time),
    /// resulting in possibly incorrect durations and lap times.
    ///
    /// <A HREF="https://en.cppreference.com/w/cpp/chrono/high_resolution_clock">
    /// std::chrono::high_resolution_clock</A> is not used because it is not implemented
    /// consistently across different standard library implementations. It is often
    /// just an alias for system_time or steady_time. For example, for gcc's libstdc++
    /// it is system_clock, for MSVC it is steady_clock, and for clang's libc++ it
    /// depends on configuration.
    /// \tparam duration_t An <A HREF="https://en.cppreference.com/w/cpp/chrono/duration">
    /// std::chrono::duration</A> type. You should consider
    /// using one of the convenience duration types, such as 
    /// <A HREF="https://en.cppreference.com/w/cpp/chrono/duration">std::chrono::nanoseconds,
    /// std::chrono::milliseconds, std::chrono::minutes</A>, and so forth.
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
            std::vector<std::chrono::nanoseconds> GetLapTimes();
            /// \brief Start timing
            /// \throws StopWatchException if you call Start() when the watch is already running.
            void Start();
            /// \brief Save timing of current lap and start timing of next lap
            void StartNextLap();
            /// \brief Stop timing
            /// \exception StopWatchException if you attempt to stop a stop watch that
            /// is not running.
            void Stop();
            ///@}
        private:
            bool m_running;
            std::chrono::time_point<std::chrono::steady_clock> m_startTime;
            std::vector<std::chrono::time_point<std::chrono::steady_clock>> m_laps;
    };
}