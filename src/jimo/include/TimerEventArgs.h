/// @file TimerEventArgs.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictaated by MIT license.
#pragma once
#include <chrono>
#include "EventArgs.h"

/// @brief A namespace containing timing related functionality
namespace jimo::timing
{
    /// @brief An EventArgs class for Timer events
    /// @tparam clock_t The std::chrono clock type to use
    template<typename clock_t>
    requires std::chrono::is_clock_v<clock_t>
    class TimerEventArgs : jimo::EventArgs
    {
        public:
            /// @brief Constructor
            TimerEventArgs() : m_time(clock_t::now()) {}
            /// @brief  Destructor
            virtual ~TimerEventArgs() {}
            /// @brief Get the time stored in the EventArgs instance.
            /// @return The time at which the TimeEventArgs instance was created.
            auto& time() { return m_time; }
        private:
            std::chrono::time_point<clock_t> m_time;
    };
}