/// \file Timer.h

#pragma once
#include "TimerException.h"
#include "Event.h"
#include "TimerEventArgs.h"
#include <chrono>
#include <thread>
#include <functional>

namespace jimo::timing
{
    using namespace std::chrono_literals;

    /// @brief Timer status
    enum class TimerStatus
    {
        /// @brief Timer has never started.
        NeverStarted,
        /// @brief Timer is currently running.
        Running,
        /// @brief Timer has stopped running.
        Stopped,
    };
    ///
    /// This class allows you to fire events once immediately, once sometime in the future,
    /// or multiple times starting immediately or sometime in the future, and multiple
    /// times.
    /// Here is an sample program:
    /// @include Timer/Timer1/Timer1.cpp
    /// @tparam clock_t A std::chrono clock.
    template<typename clock_t>
    requires std::chrono::is_clock_v<clock_t>
    class Timer : public Object
    {
        public:
            /// @brief Constructor
            Timer() : m_status(TimerStatus::NeverStarted) {}
            /// @brief Destructor
            virtual ~Timer() noexcept {}
            /// @brief tick event.
            ///
            /// This is the event that fires every time the timer "expires".
            /// @note The event handlers attached to this event are executed on a different
            /// thread than the ones that added handlers to this event.
            Event<Timer, TimerEventArgs<clock_t>> tick; 
            /// @brief Run the timer to fire once at the specified start time.
            /// @param startTime The time at which to execute the tick event handlers.
            /// If this time is *now* or in the past, the tick event fires immediately.
            /// If this time is in the future, the timer waits until or after that time
            /// to fire the tick event.
            /// @exception TimerException if called on an already running timer.
            void run(const std::chrono::time_point<clock_t>& startTime)
            {
                run(startTime, 0us, 1);
            }
            /// @brief Run the timer to fire at the specified interval.
            ///
            /// Timer never "expires". Well it will if run long enough, but that is a huge
            /// amount of time.
            /// @param timerInterval The time interval between firings of the timer. This
            /// time cannot be less than microseconds in duration.
            /// @exception TimerException if called on an already running timer.
            /// @note To stop the timer, call Timer::stop.
            /// @note If the interval is too short, there will not be enough time to
            /// run event handlers. In this case, the event fires immediately upon
            /// completion of the previous fire event.
            void run(const std::chrono::microseconds& timerInterval)
            {
                run(clock_t::now() + timerInterval, timerInterval, -1);
            }
            /// @brief Run the timer to fire at the specified interval for the specified
            /// number of times.
            /// @param timerInterval The time interval between firings of the timer. This
            /// time cannot be less than milliseconds in duration.
            /// @param count The number of times to fire. It may have the following values:
            ///
            ///> >0 - fire this number of times.
            ///
            ///> 0 - never fire.
            ///
            ///> -1 - fire until Timer::stop is called.
            /// @exception TimerException if called on an already running timer.
            /// @note If the interval is too short, there will not be enough time to
            /// run event handlers. In this case, the event fires again immediately upon
            /// completion of the previous fire event.
            void run(const std::chrono::microseconds& timerInterval, long long count)
            {
                run(clock_t::now() + timerInterval, timerInterval, count);
            }
            /// @brief Run the timer to fire at the specified time, and at the specified
            /// interval thereafter for the specified number of times.
            /// @param startTime The time that the timer should fire the first time 
            /// @param timerInterval The time interval between firings of the timer. This
            /// time cannot be less than milliseconds in duration.
            /// @param count The number of times to fire. It may have the following values:
            ///
            ///> >0 - fire this number of times.
            ///
            ///> 0 - never fire.
            ///
            ///> -1 - fire until Timer::stop is called.
            /// @exception TimerException if called on an already running timer.
            /// @note If the start time is in the past, the tick event will fire
            /// continuously until <code>startTime += timerInterval</code> is greater than the 
            /// current time, or the tick event has fired <code>count</code> times.
            /// @note If the interval is too short, there will not be enough time to
            /// run event handlers. In this case, the event fires again immediately upon
            /// completion of the previous fire event.
            void run(const std::chrono::time_point<clock_t>& startTime,
                const std::chrono::microseconds& timerInterval, long long count)
            {
                if (m_status == TimerStatus::NeverStarted)
                {
                    m_status = TimerStatus::Running;
                    m_timerCount = count;
                    m_interval = timerInterval;
                    m_timeToFireEvent = startTime;
                    m_timerThread = std::make_unique<std::jthread>(
                        std::mem_fn(&Timer::runTimer), this);
                }
                else if (m_status == TimerStatus::Running)
                {
                    stop();
                    throw TimerException("Timer is already running.");
                }
            }
            /// @brief Stop the timer.
            ///
            /// @note The timer does not stop immediately; it waits until the next time
            /// that the timer expires before actually stopping the timer. The tick event
            /// handlers do not run at that time.
            /// @note At the moment, the timer does not actually stop. This functionality
            /// will be added later.
            /// @exception TimerException if run has never been called.
            void stop()
            {
                if (m_status == TimerStatus::NeverStarted)
                {
                    throw TimerException(
                        "Timer::stop called for a timer that was never started.");
                }
                m_timerCount = 0;
                m_status = TimerStatus::Stopped;
            }
        private:
            void runTimer()
            {
                while (m_timerCount != 0)
                {
                    auto timeToWait = m_timeToFireEvent - clock_t::now();
                    if (timeToWait > 0ns)
                    {
                        std::this_thread::sleep_for(timeToWait);
                    }
                    if (m_status == TimerStatus::Running)
                    {
                        onTick(TimerEventArgs<clock_t>());
                        m_timeToFireEvent += m_interval;
                        --m_timerCount;
                    }
                }
                m_status = TimerStatus::Stopped;
            }
            void onTick(const TimerEventArgs<clock_t>& e)
            {
                tick(*this, e);
            }
            TimerStatus m_status;
            long long m_timerCount { 0 };
            // does not compile if m_interval is nanoseconds
            std::chrono::microseconds m_interval { 1s };
            std::chrono::time_point<clock_t> m_timeToFireEvent;
            std::unique_ptr<std::jthread> m_timerThread;
    };

}