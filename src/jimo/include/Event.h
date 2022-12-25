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
    /// \include Event/Event.cpp
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
            virtual auto equals(const Event& other) const -> bool
            {
                return *this == dynamic_cast<const EventHandler<sender_t, eventArgs_t>&>(other);
            }
    };
}
