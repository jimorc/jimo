///
/// \file StopWatch.h
///
#pragma once
#import <chrono>
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
    template<typename duration_t>
    class StopWatch
    {
        public:
            /// \name Constructors and Copy/Move Operators
            ///@{

            /// \brief The default StopWatch constructor
            StopWatch() {}
            /// \brief Copy constructor
            StopWatch(const StopWatch&) = delete;
            /// \brief Move constructor
            StopWatch(StopWatch&&) = delete;
            /// \brief Copy operator=
            StopWatch& operator =(const StopWatch&) = delete;
            /// \brief Move operator=
            StopWatch& operator =(StopWatch&&) = delete;
            ///@}
            /// \name Properties
           ///@{
            ///
            /// \brief Retrieve the duration between the calls to Start() and Stop().
            /// \returns The difference between the start and stop times. 
            duration_t Duration() const noexcept
            {
                return m_stopTime - m_startTime;
            }
            ///@}
            ///
            /// \name Methods
           ///@{
            ///
            /// \brief Start the stop watch
            /// \warning Calliing Start() simply resets the start time to the current time.
            void Start()
            {
                m_startTime = std::chrono::steady_clock::now();
            }
            /// \brief Stop the stop watch
            /// \warning Calling Stop() simply sets the stop time to the current time.
            /// No check is performed to determine if Start() was called first.
            void Stop() noexcept
            {
                m_stopTime = std::chrono::steady_clock::now();
            }
            ///@}
        private:
            std::chrono::time_point<std::chrono::steady_clock> m_startTime;
            std::chrono::time_point<std::chrono::steady_clock> m_stopTime;
    };
}