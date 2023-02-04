/// @file ActionHandler.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>
#include <exception>
#include <format>
#include <functional>
#include <initializer_list>
#include "Action.h"

using namespace std::placeholders;

/// @namespace threading
/// @brief The namespace for all enumerations, structs, and classes related to threading,
/// including Action, ActionHandler, and ThreadedActionHandler.
namespace jimo::threading
{
    /// @concept ActionEnum    
    /// @brief Concept for action enumerations used with the Action struct and the
    /// ActionHandler and ThreadedActionHandler classes.
    /// @tparam actionEnum_t The action enumeration type
    /// @note The enumeration must contain values for `terminate`, `runContinuous`, and
    /// `stopContinuous` to be valid. 
    /// @note There are handlers already included in the
    /// ActionHandler class for these actions.
    /// @see ActionHandler
    /// @see ThreadedActionHandler
    template<typename actionEnum_t>
    concept ActionEnum = requires
    {
        std::is_enum_v<actionEnum_t>;
        actionEnum_t::terminate;
        actionEnum_t::runContinuous;
        actionEnum_t::stopContinuous;
    };
    /// @brief ActionHandler class
    /// @tparam actionEnum_t The enumeration containing the various actions that can
    /// be handled by this ActionHandler
    /// @see ActionEnum concept
    /// @note This handler class executes under the same thread as the run() method is 
    /// called in.
    /// The run() method is like a message processor function in a GUI program. It continues
    /// to execute until either a `terminate` action is handled, or an exception is
    /// thrown. Therefore, if there is only one thread within the program that contains
    /// an object of this class, the `terminate` action must be queued from another handler 
    /// within
    /// this ActionHandler, or must be called from within the continuouslyRun() method.
    /// If there is more than one thread in the program, then a `terminate` action could
    /// potentially be queued from within one of the other threads.
    /// @note This class is thread-safe. That is, no race conditions are present unless
    /// they are introduced in the handlers added to any objects of this class.
    template <ActionEnum actionEnum_t>
    class ActionHandler
    {
        public:
            /// @brief Definition of an Action entry.
            ///
            /// This is used to define the input to the addHandlers() method.
            using ActionEntry = std::pair<actionEnum_t, std::function<void(Action<actionEnum_t>&)>>;
            /// @brief The definition for the map used to contain the list of actions that the 
            /// ActionHandler can perform.
            using ActionMap = std::map<actionEnum_t, std::function<void(Action<actionEnum_t>&)>>;
            /// @brief Default constructor
            ActionHandler()
            {
                addHandlers({
                    {actionEnum_t::terminate, std::bind(
                        &ActionHandler<actionEnum_t>::terminateHandler, this, _1)},
                    {actionEnum_t::runContinuous, std::bind(
                        &ActionHandler<actionEnum_t>::startContinuousRun, this, _1)},
                    {actionEnum_t::stopContinuous, std::bind(
                        &ActionHandler<actionEnum_t>::stopContinuousRun, this, _1)} 
                }); 
            }
            /// @brief Destructor
            virtual ~ActionHandler() = default;
            /// @brief Queue an action for processing.
            /// @param action The Action struct defining the action to perform.
            void queueAction(const Action<actionEnum_t>& action) noexcept
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_actionQueue.push(action);
                m_actionQueued = true;
                m_actionInQueue.notify_one();
            }
            /// @brief Add action handlers.
            ///
            /// Each action handler specifies how to handle a specific action request.
            /// @param actions Handlers for the various actions.
            void addHandlers(std::initializer_list<ActionEntry> actions)
            {
                for (const auto& action : actions)
                {
                    m_actionMap.insert(action);
                }
            }
            /// @brief Run the actions requested of this ActionHandler.
            ///
            /// Functionality is as follows:
            /// * Actions that have been queued are processed in the order that they were
            /// queued (first-in-first-out).
            /// * Once all actions have been performed and if a `runContinuous` action
            /// has been processed, then the defined continuous action is
            /// executed. When the continuous action has been executed once, the action
            /// queue is again checked for actions to perform.
            /// * If there are no actions in the queue and there is no continuous action
            /// to perform, the handler waits for the next action to be queued.
            /// @exception std::out_of_range exception if no handler has been added for
            /// the action defined by the action enumeration value.
            void run()
            {
                std::unique_lock<std::mutex> lock(m_queueMutex, std::defer_lock);
                while (true)
                {
                    if (m_terminate) return;
                    if (m_actionQueued)
                    {
                        lock.lock();
                        Action<actionEnum_t> action = m_actionQueue.front();
                        m_actionQueue.pop();
                        if (m_actionQueue.empty()) m_actionQueued = false;
                        lock.unlock();
                        auto function_iter = m_actionMap.find(action.action);
                        if (function_iter != m_actionMap.end())
                        {
                            function_iter->second(action);
                            continue;
                        }
                        else
                        {
                            std::string errMessage = std::format(
                                "The Action with enumeration value {} has no handler.",
                                static_cast<int>(action.action));
                            throw std::out_of_range(errMessage);
                        }
                    }
                    if (m_runContinuously) continuouslyRun();
                    else
                    {
                        lock.lock();
                        if (!m_actionQueued) m_actionInQueue.wait(lock);
                        lock.unlock();
                    }
                }
            }
        protected:
            /// @brief The method to run continuously (after the `runContinuous` action
            /// has been processed).
            ///
            /// As this is a pure virtual method, the derived class must override this 
            /// method even if no continuous action is to be performed by the
            /// derived ActionHandler.
            virtual void continuouslyRun() = 0;
        private:
            void startContinuousRun(Action<actionEnum_t>&)
            {
                m_runContinuously = true;
            }
            void stopContinuousRun(Action<actionEnum_t>&)
            {
                m_runContinuously = false;
            }
            void terminateHandler(Action<actionEnum_t>&)
            {
                m_terminate = true;
            }
            std::atomic_bool m_runContinuously {};
            std::atomic_bool m_actionQueued {};
            std::atomic_bool m_terminate {};
            std::queue<Action<actionEnum_t>> m_actionQueue;
            std::mutex m_queueMutex;
            std::condition_variable m_actionInQueue {};
            ActionMap m_actionMap;
    };
}

