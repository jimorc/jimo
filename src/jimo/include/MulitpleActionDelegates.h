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
    /// This class holds multiple jimo::Delegate entries that are stored indexed on
    /// the enumeration.
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
            void addToDelegates(enum_t actionType, const std::function<void(std::any)>& actionFunction)
            {
                m_multiActions[actionType] += actionFunction;
            }
            /// @brief Add a jimo::Delegate
            /// @param actionType The action enumeration that indexes the jimo::Delegate
            /// that the delegates are to be added to.
            /// @param delegate the jimo::Delegate to add to the indexed delegates.
            void addToDelegates(enum_t actionType, const jimo::Delegate<void, std::any>&
                delegate)
            {
                m_multiActions[actionType] += delegate;
            }
            /// @brief Retrieve the jimo::Delegate object that is indexed by the argument.
            /// @param actionType The action enumeration value for which the jimo::Delegate
            /// is to be retrieved.
            /// @return The jimo::Delegate object.
            /// @exception std::out_of_range exception if the actionType does not exist
            /// in this MultipleActionDelegates object.
            auto& operator [](enum_t actionType)
            {
                return m_multiActions.at(actionType);
            }
        private:
            std::map<enum_t, jimo::Delegate<void, std::any>> m_multiActions;
    };
}