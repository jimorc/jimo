/// @file EventHandler.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#pragma once
#include "Delegate.h"
#include "Object.h"

namespace jimo
{
    /// @brief A specialized Delegate used for events.
    ///
    /// @see Event
    /// @tparam sender_t The type of the object that invokes the event. This type must be
    /// Object, or derived from Object.
    /// @tparam eventArgs_t The class type of the object containing arguments that are
    /// passed to the event.
    template<typename sender_t, typename eventArgs_t>
    using EventHandler = Delegate<void, sender_t&, eventArgs_t&>;
}