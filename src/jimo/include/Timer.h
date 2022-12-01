/// \file Timer.h

#pragma once
#include "TimerException.h"
#include <chrono>
#include <thread>
#include <functional>

namespace jimo::timing
{
    /// \typedef VoidFunction
    /// \brief typedef for the function or method to call when the Timer "fires"'
    using VoidFunction = void(*)();

    /// \enum TimerStatus
    /// \brief An enumeration that specifies the status of a timer
    enum class TimerStatus
    {
        NeverStarted,
        ///< Timer has been constructed but not fired
        Running,
        ///< Timer is in a running state.
        Stopped
        ///< Timer has stopped
    };
    /// \class OnceTimer
    /// \brief A timer that expires once only.
    ///
    /// The timer may expire and fire an "event" once immediately or sometime in the future.
    /// \warning When OnceTimer::Fire is called, a new thread is started. The "event" executes
    /// in that new thread, not the thread that called the Fire method.
    ///
    /// Here is a program that illustrates the use of the OnceTimer class:
    /// \include Timer/OnceTimer/OnceTimer.cpp
    template<typename clock_t>
    requires std::chrono::is_clock_v<clock_t>
    class OnceTimer
    {
        public:
            /// \brief Constructor
            /// \param func The function to call when the OnceTimer "fires".
            OnceTimer(VoidFunction func) : m_func(func), m_timerCount(0), 
                m_status(TimerStatus::NeverStarted) {}
            /// \brief Fire the timer.
            ///
            /// A OnceTimer can only be fired once.
            /// \param startTime The time that the timer is to fire at. If startTime is earlier
            /// than now, the timer fires immediately.
            /// \exception TimerException thrown if OnceTimer::Fire is called more than once.
            void Fire(std::chrono::time_point<clock_t> startTime)
            {
                if (m_status == TimerStatus::NeverStarted)
                {
                    m_status = TimerStatus::Running;
                    m_startTime = startTime;
                    m_timerCount = 1;
                    m_timerThread = std::make_unique<std::jthread>(std::mem_fn(&OnceTimer::SingleTime), this);
                }
                else
                {
                    m_status = TimerStatus::Stopped;
                    throw TimerException("OnceTimer::Fire can only be called once.");
                }
            }
            /// \brief Stop a running OnceTimer
            /// \exception TimerException thrown if Stop is called before Fire is called
            void Stop()
            {
                if (m_status == TimerStatus::NeverStarted)
                {
                    throw TimerException("OnceTimer::Stop called for a timer that was never started.");
                }
                
                m_status = TimerStatus::Stopped;
                m_timerCount = 0;
                m_timerThread->request_stop();
            }
        private:
            void SingleTime()
            {
                auto waitTime = m_startTime - clock_t::now();
                if (waitTime > 0ns)
                {
                    std::this_thread::sleep_for(waitTime);
                }
                if (m_status == TimerStatus::Running)
                {
                    (*m_func)();
                }
                m_timerCount = 0;
                m_status = TimerStatus::Stopped;
            }
            VoidFunction m_func;
            std::chrono::time_point<clock_t> m_startTime;
            std::unique_ptr<std::jthread> m_timerThread;
            long m_timerCount;
            TimerStatus m_status;
    };
}