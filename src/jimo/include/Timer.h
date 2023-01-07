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

    /// @brief A Timer class.
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
            Timer() {}
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
            void run(const std::chrono::time_point<clock_t>& startTime)
            {
                if (m_status == TimerStatus::NeverStarted)
                {
                    m_status = TimerStatus::Running;
                    m_timerCount = 1;
                    m_timeToFireEvent = startTime;
                    m_timerThread = std::make_unique<std::jthread>(
                        std::mem_fn(&Timer::runTimer), this);
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
            }
        private:
            void runTimer()
            {
                auto timeToWait = m_timeToFireEvent - clock_t::now();
                if (timeToWait > 0ns)
                {
                    std::this_thread::sleep_for(timeToWait);
                }
                if (m_status == TimerStatus::Running)
                {
                    onTick(TimerEventArgs<clock_t>());
                }
                m_status = TimerStatus::Stopped;
            }
            void onTick(const TimerEventArgs<clock_t>& e)
            {
                tick(*this, e);
            }
            TimerStatus m_status { TimerStatus::NeverStarted };
            long m_timerCount { 0 };
            std::chrono::time_point<clock_t> m_timeToFireEvent;
            std::unique_ptr<std::jthread> m_timerThread;
    };

}