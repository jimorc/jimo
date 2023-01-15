/// @file Event.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#pragma once
#include "EventArgs.h"
#include "Object.h"
#include "EventHandler.h"
#include <concepts>
#include <type_traits>

namespace jimo
{
    /// @brief A specialized event handler derived from Delegate.
    /// @tparam sender_t The type of the object that invokes the handler.
    /// @tparam eventArgs_t The type of the event arguments passed to the event handler.
    /// The type must be either jimo::EventArgs, or a type that is derived from EventArgs.
    ///
    /// Here is a program that illustrates the use of the Event class:
    /// @include Event/Event1/Event1.cpp
    template<typename sender_t, typename eventArgs_t>
    requires std::derived_from<eventArgs_t, EventArgs>
    class Event : public EventHandler<sender_t, eventArgs_t>
    {
        public:
            /// @brief Constructor
            Event() = default;
            /// @brief Destructor
            virtual ~Event() noexcept = default;
            /// @brief Compare two Event objects for equality.
            /// @param other The Event object to compare with this.
            /// @return <code>true</code> if the objects are equal, <code>false</code> otherwise.
            virtual auto equals(const Event& other) const noexcept -> bool
            {
                return *this == other;
            }
            /// @brief Invoke the methods represented by the current event.
            /// @param sender The object that called invoke.
            /// @param e an event args object. It must be derived from EventArgs.
            virtual void invoke(sender_t& sender, eventArgs_t& e)
            {
                operator ()(sender, e);
            }
            /// @brief Invoke the methods represented by the current event.
            /// @param sender The object that called invoke.
            /// @param e an event args object. It must be derived from EventArgs.
            virtual void operator ()(sender_t& sender, eventArgs_t& e)
            {
                if (EventHandler<sender_t, eventArgs_t>::m_data->functions.empty())
                {
                    return;
                }
                std::vector<EventHandler<sender_t, eventArgs_t>::function_t> functions;
                auto& event = const_cast<Event&>(*this);
                {
                    std::lock_guard<std::mutex> lock(event.m_functionsLock);
                    functions = event.m_data->functions;
                }
                for (size_t index = 0; index < functions.size(); ++index)
                {
                    functions[index](sender, e);
                    if(e.halt()) return;
                }
            }
    };
}
