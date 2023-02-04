/// @file Action.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <any>
#include <type_traits>
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
        /// @brief Default constructor
        Action() = default;
        /// @brief Constructor that defines an action and the data to pass to the handler.
        /// @param act The action to perform.
        /// @param data The data to pass to the action handler.
        /// @note No callback is specified. This can be added just as you could if you
        /// had started with the default constructor.
        Action(actionEnum_t act, const std::any& data)
            : action(act), actionData(data) {}
        /// @brief Constructor that defines an action, the data to pass to the handler,
        /// and a callback to execute once the action has been handled.
        /// @param act The action to perform.
        /// @param data The data to pas to the action handler.
        /// @param callback The callback function to execute once the action has been
        /// executed.
        /// @note The callback is not called automatically. It must be explicitly called
        /// from within the action handler.
        Action(actionEnum_t act, const std::any& data, 
            const std::function<void(std::any)>& callback)
            : action(act), actionData(data), actionCallback(callback) {}
        /// @brief Constructor that defines an action, the data to pass to the handler,
        /// and a callback to execute once the action has been handled.
        /// @param act The action to perform.
        /// @param data The data to pas to the action handler.
        /// @param callbacks The callback functions to execute once the action has been
        /// executed.
        /// @note The callbacks are not called automatically. They must be explicitly called
        /// from within the action handler.
        Action(actionEnum_t act, const std::any& data, 
            const jimo::Delegate<void, std::any>& callbacks)
            : action(act), actionData(data), actionCallback(callbacks) {}
        /// @brief Copy constructor
        /// @param other The Action object to copy.
        Action(const Action& other) noexcept
            : action(other.action), actionData(other.actionData), 
                actionCallback(other.actionCallback) {}
        /// @brief Move constructor
        /// @param other The Action object to move.
        Action(Action&& other) noexcept
            : action(std::move(other.action)), actionData(std::move(other.actionData)),
                actionCallback(std::move(other.actionCallback)) 
        {
            other.action = {};
            other.actionData = {};
            other.actionCallback.clear();
        }
        /// @brief Destructor
        virtual ~Action() noexcept = default;
        /// @brief Copy equals operator
        /// @param other The Action object to copy.
        /// @return The copied Action object.
        Action& operator =(const Action& other) noexcept
        {
            action = other.action;
            actionData = other.actionData;
            actionCallback = other.actionCallback;
            return *this;
        };
        /// @brief Move equals operator
        /// @param other The Action object to move.
        /// @return The moved Action object.
        Action& operator =(Action&& other) noexcept
        {
            action = other.action;
            actionData = other.actionData;
            actionCallback = other.actionCallback;
            other.action = {};
            other.actionData = {};
            other.actionCallback.clear();
            return *this;
        }
        /// @brief The action that the ActionHandler or threadedActionHandler is to perform.
        /// @note The handler for the action is defined in the actionHandler or 
        /// threadedActionHandler, not
        /// in the `Action` struct. Potentially, an action could be specified in the
        /// actionData, but there still must be some code defined that interprets the
        /// actionData to call that action. This is not the intended use of the Action
        /// struct.
        actionEnum_t action;
        /// @brief Data to be passed to the method in the ActionHandler or
        /// ThreadedActionHandler.
        /// @note The actionData type must be copyable and movable. If not, then a
        /// compilation error will be generated when attempting to set this value.
        std::any actionData {};
        /// @brief Any callbacks that are to be called after the action is completed.
        /// @note The callbacks are not called automatically after the action handler is
        /// executed. For the callbacks to be executed, the handler associated with this
        /// action must call them explicitly.
        jimo::Delegate<void, std::any> actionCallback;
    };
}