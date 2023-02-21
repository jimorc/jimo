/// @file MultipleActionDelegates.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <map>
#include <functional>
#include <any>
#include "Delegate.h"

namespace jimo::threading
{
    /// @brief MultipleActionDelegates class
    ///
    /// This class is a container for multiple jimo::Delegate<void, std::any> entries.
    /// @tparam enum_t The enumeration that is used to index the various jimo::Delegates.
    template<typename enum_t>
    requires std::is_enum_v<enum_t>
    class MultipleActionDelegates
    {
        public:
            /// @brief Default constructor
            MultipleActionDelegates() {}
            /// @brief Destructor
            virtual ~MultipleActionDelegates() = default;
            /// @brief Add a delegate function.
            /// @param actionType The action enumeration that indexes the jimo::Delegate
            /// to store the function into.
            /// @param actionFunction The action function to add to the delegate.
            /// @return The MultipleActionDelegates with the function added.
            MultipleActionDelegates& addToDelegates(enum_t actionType, const std::function<void(std::any)>& actionFunction)
            {
                m_multiActions[actionType] += actionFunction;
                return *this;
            }
            /// @brief Add a jimo::Delegate
            /// @param actionType The action enumeration that indexes the jimo::Delegate
            /// that the delegates are to be added to.
            /// @param delegate the jimo::Delegate to add to the indexed delegates.
            /// @return The MultipleActionDelegates with the functions in the Delegate added.
            MultipleActionDelegates& addToDelegates(enum_t actionType, const jimo::Delegate<void, std::any>&
                delegate)
            {
                m_multiActions[actionType] += delegate;
                return *this;
            }
            /// @brief Erase all elements from the container.
            ///
            /// After this call, size() returns 0, and empty() returns true;
            void clear() noexcept
            {
                m_multiActions.clear();
            }
            /// @brief Checks if the container has no elements.
            /// @return `true` if the container is empty, `false` otherwise.
            [[nodiscard]] bool empty() const noexcept
            {
                return m_multiActions.empty();
            }
            /// @brief Remove the function from the indexed Delegate.
            /// @param actionType The action enumeration that indexes the jimo::Delegate
            /// that the function is removed from.
            /// @param actionFunction The function to remove.
            /// @return The MultipleActionDelegates with the function removed.
            /// @note All instances of the function are removed from the jimo::Delegate.
            /// @note No action is taken if the function is not present in the specified
            /// jimo::Delegate.
            /// @exception std::out_of_range exception if no entry exists
            /// for the actionType. No exception is thrown if an empty entry exists.
            MultipleActionDelegates& removeFromDelegates(enum_t actionType, const std::function<void(std::any)>& actionFunction)
            {
                m_multiActions.at(actionType) -= actionFunction;
                return *this;
            }
            /// @brief Remove all functions in the jimo::Delegate parameter from the
            /// indexed jimo::Delegate.
            /// @param actionType The action enumeration that indexes the jimo::Delegate
            /// to remove the functions from.
            /// @param delegate The jimo::Delegate containing the functions to remove.
            /// @return The MultipleActionDelegates with the functions in the Delegate
            /// parameter removed.
            /// @note All instances of all functions in the Delegate parameter are
            /// removed from the indexed jimo::Delegate.
            /// @note If the indexed jimo::Delegate does not contain a function in the
            /// Delegate argument, then no action is taken for than function.
            /// @exception std::out_of_range exception if no entry exists
            /// for the actionType. No exception is thrown if an empty entry exists.
            MultipleActionDelegates& removeFromDelegates(enum_t actionType, const jimo::Delegate<void, std::any>&
                delegate)
            {
                m_multiActions.at(actionType) -= delegate;
                return *this;
            }
            /// @brief Retrieve the jimo::Delegate object that is indexed by the argument.
            /// @param actionType The action enumeration value for which the jimo::Delegate
            /// is to be retrieved.
            /// @return The jimo::Delegate object.
            /// @exception std::out_of_range exception if the actionType does not exist
            /// in this MultipleActionDelegates object.
            Delegate<void, std::any>& operator [](enum_t actionType)
            {
                return m_multiActions.at(actionType);
            }
        private:
            std::map<enum_t, jimo::Delegate<void, std::any>> m_multiActions;
    };
}