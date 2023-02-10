#include <iostream>
#include <thread>
#include "Action.h"
#include "ActionHandler.h"
#include "ThreadedActionHandler.h"

using namespace jimo::threading;
using namespace std::chrono_literals;

enum class ThreadedHandlerActions
{
    runContinuous = 1,
    stopContinuous,
    terminate,
    twoTimesValue,
};

enum class MainHandlerActions
{
    runContinuous = 1,
    stopContinuous,
    terminate,
    startTheadedHandler,
    setValueViaThreadedHandler,
    setValue,
    terminateThreadedHandler,
};

class ThreadedHandler : public ThreadedActionHandler<ThreadedHandlerActions>
{
    public:
        ThreadedHandler() : ThreadedActionHandler<ThreadedHandlerActions>()
        {
            addHandlers( { {ThreadedHandlerActions::twoTimesValue, std::bind(
                &ThreadedHandler::twoTimesValue, this, _1) }
            });
        }
        ~ThreadedHandler() = default;
        void twoTimesValue(Action<ThreadedHandlerActions>& action)
        {
            int timesTwo = std::any_cast<int>(action.actionData) * 2;
            action.actionCallback(timesTwo);
        }
    protected:
        void continuouslyRun() override { std::this_thread::sleep_for(25ms); }
    private:
        int m_value {};
        int m_value2 {};
};
class MainHandler : public ActionHandler<MainHandlerActions>
{
    public:
        MainHandler() : ActionHandler<MainHandlerActions>()
        {
            addHandlers({ { MainHandlerActions::startTheadedHandler, std::bind(
                            &MainHandler::startThreadedHandler, this, _1) },
                            { MainHandlerActions::setValueViaThreadedHandler,
                            std::bind(&MainHandler::setViaThreadedHandler, this, _1) },
                            { MainHandlerActions::setValue, std::bind(
                            &MainHandler::setValue, this, _1) },
                            { MainHandlerActions::terminateThreadedHandler,
                            std::bind(&MainHandler::terminateThreadedHandler, this, _1)}
            });
            queueAction(Action<MainHandlerActions>(MainHandlerActions::startTheadedHandler, 
                {}));
            queueAction(Action<MainHandlerActions>(MainHandlerActions::runContinuous, {}));
        }
        virtual ~MainHandler() = default;
        void startThreadedHandler(Action<MainHandlerActions>&)
        {
            if (!m_tHandler)
            {
                m_tHandler = std::make_unique<ThreadedHandler>();
                m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                    ThreadedHandlerActions::runContinuous, {}));
            }
        }
        void terminateThreadedHandler(Action<MainHandlerActions>&)
        {
            if (m_tHandler)
            {
                m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                    ThreadedHandlerActions::stopContinuous, {}));
                m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                    ThreadedHandlerActions::terminate, {}));
            }
        }
        void setViaThreadedHandler(Action<MainHandlerActions>& action)
        {
            m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                ThreadedHandlerActions::twoTimesValue, action.actionData,
                *this, &MainHandler::setValueInHandler));
        }
        void setValue(Action<MainHandlerActions>& action)
        {
            m_value = std::any_cast<int>(action.actionData);
            std::cout << "Value has been set to " << m_value << '\n';
        }
        void setValueInHandler(std::any value)
        {
            queueAction(Action<MainHandlerActions>(MainHandlerActions::setValue,
                { value }));
        }
        int value() const noexcept { return m_value; }
    protected:
        void continuouslyRun() override
        {
            if (++m_count % 4 == 0)
            {
                queueAction(Action<MainHandlerActions>(
                    MainHandlerActions::setValueViaThreadedHandler, m_count));
            }
            // check against 21 to give time for ThreadedHandler to process action above
            // for m_count of 20.
            if (m_count <= 21)
            {
                std::this_thread::sleep_for(50ms);
            }
            else
            {
                queueAction(Action<MainHandlerActions>(
                    MainHandlerActions::terminateThreadedHandler, {}));
                queueAction(Action<MainHandlerActions>(MainHandlerActions::stopContinuous,
                {}));
                queueAction(Action<MainHandlerActions>(MainHandlerActions::terminate,
                {}));
            }
        }
    private:
        std::unique_ptr<ThreadedHandler> m_tHandler;
        int m_value {};
        int m_count {};
};

int main()
{
    MainHandler handler;
    handler.run();
}

