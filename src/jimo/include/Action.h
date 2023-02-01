/// @file Action.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <any>
#include "Delegate.h"

namespace jimo::threading
{
    /// @brief Action struct
    ///
    /// The Action struct specifies an action to be performed by an ActionHandler or
    /// ThreadActionHandler object. The actual functionality is defined in the ActionHandler
    /// or ThreadActionHandler as specified by the action enum value. Once the `action`
    /// has been performed, the actionCallback Delegate is called to inform others,
    /// including potentially the action caller, that the action has been performed, and
    /// to return the result of action.
    /// @tparam actionEnum_t The enumeration that specifies the action to be performed.
    template<typename actionEnum_t>
    requires std::is_enum_v<actionEnum_t>
    struct Action
    {
        /// @brief The action that the ActionHandler or ThreadActionHandler is to perform.
        /// @note The action is defined in the actionHandler or ThreadActionHandler, not
        /// in the `Action` struct. Potentially, an action could be specified in the
        /// actionData, but there still must be some code defined that interprets the
        /// actionData to call that action. This is not the intended use of the Action
        /// struct.
        actionEnum_t action;
        /// @brief Data to be passed to the method in the ActionHandler or
        /// ThreadActionHandler.
        std::any actionData {};
        /// @brief Any callbacks that are to be called after the action is completed.
        jimo::Delegate<void, std::any> actionCallback;
    };
}