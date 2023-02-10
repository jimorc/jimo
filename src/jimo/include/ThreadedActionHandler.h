/// @file ThreadedActionHandler.h
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <thread>
#include <exception>
#include "ActionHandler.h"

namespace jimo::threading
{
    /// @brief ThreadedActionHandler class
    ///
    /// ThreadedActionHandler is similar to ActionHandler except that it runs under its own
    /// thread.
    /// @tparam actionEnum_t The Actions enumeration class for this handler
    template<typename actionEnum_t>
    class ThreadedActionHandler : public ActionHandler<actionEnum_t>
    {
        public:
            /// @brief Default constructor
            ///
            /// This constructor calls run() in a separate thread.
            ThreadedActionHandler() : ActionHandler<actionEnum_t>() 
            {
                // must start ActionHandler::run() because ThreadedActionHandler::run
                // throws exception.
                m_thread = std::jthread(
                    [this]() { ActionHandler<actionEnum_t>::run(); });
            }
            /// @brief Destructor
            virtual ~ThreadedActionHandler() noexcept = default;
            /// @brief Run the action handler.
            ///
            /// @note This method must not be called. All it does is terminate the
            /// ThreadedActionHandler and throw a logic_error. This is because the
            /// correct run() method is started automatically in the ThreadedActionHandler
            /// constructor.
            /// @exception std::logic_error if this method is called in your code.
            void run() override
            {
                // must queue terminate action or handler never stops
                this->queueAction(Action<actionEnum_t>(actionEnum_t::terminate, {}));
                throw std::logic_error("ThreadedActionHandler is already running.");
            }
        private:
            std::jthread m_thread;
    };
}