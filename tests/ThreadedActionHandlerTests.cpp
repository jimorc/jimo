#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "Action.h"
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
};

TEST(ThreadedActionHandlerTests, testRunStopTerminate)
{
    class ThreadedHandler : public ThreadedActionHandler<ThreadedHandlerActions>
    {
        public:
            ThreadedHandler() : ThreadedActionHandler<ThreadedHandlerActions>() {}
            ~ThreadedHandler() = default;
        protected:
            void continuouslyRun() override { std::this_thread::sleep_for(25ms); }
    };

    ThreadedHandler handler;
    handler.queueAction(Action<ThreadedHandlerActions>(ThreadedHandlerActions::runContinuous, {}));
    std::this_thread::sleep_for(250ms);
    handler.queueAction(Action<ThreadedHandlerActions>(ThreadedHandlerActions::stopContinuous, {}));
    handler.queueAction(Action<ThreadedHandlerActions>(ThreadedHandlerActions::terminate, {}));
    SUCCEED();
}

TEST(ThreadedActionHandlerTests, testCrossThreadCommunications)
{
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
    class Handler : public ActionHandler<MainHandlerActions>
    {
        public:
            Handler() : ActionHandler<MainHandlerActions>()
            {
                addHandlers({ { MainHandlerActions::startTheadedHandler, std::bind(
                                &Handler::startThreadedHandler, this, _1) },
                              { MainHandlerActions::setValueViaThreadedHandler,
                                std::bind(&Handler::setViaThreadedHandler, this, _1) },
                              { MainHandlerActions::setValue, std::bind(
                                &Handler::setValue, this, _1) }
                });
            }
            virtual ~Handler() = default;
            void startThreadedHandler(Action<MainHandlerActions>&)
            {
                if (!m_tHandler)
                {
                    m_tHandler = std::make_unique<ThreadedHandler>();
                }
            }
            void setViaThreadedHandler(Action<MainHandlerActions>& action)
            {
                m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                    ThreadedHandlerActions::twoTimesValue, action.actionData,
                    *this, &Handler::setValueInHandler));
            }
            void setValue(Action<MainHandlerActions>& action)
            {
                m_value = std::any_cast<int>(action.actionData);
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
                if (++m_count < 20)
                {
                    std::this_thread::sleep_for(50ms);
                }
                else
                {
                    m_tHandler->queueAction(Action<ThreadedHandlerActions>(
                        ThreadedHandlerActions::terminate, {}));
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

    Handler handler;
    handler.queueAction(Action<MainHandlerActions>(MainHandlerActions::startTheadedHandler, 
        {}));
    handler.queueAction(Action<MainHandlerActions>(MainHandlerActions::setValueViaThreadedHandler,
        { 3 }));
    handler.queueAction(Action<MainHandlerActions>(MainHandlerActions::runContinuous, {}));
    handler.run();
    ASSERT_EQ(6, handler.value());
}

TEST(ThreadedActionHandlerTests, testRunCalled)
{
    class ThreadedHandler : public ThreadedActionHandler<ThreadedHandlerActions>
    {
        public:
            ThreadedHandler() : ThreadedActionHandler<ThreadedHandlerActions>() {}
            ~ThreadedHandler() = default;
        protected:
            void continuouslyRun() override { std::this_thread::sleep_for(25ms); }
    };

    try
    {
        ThreadedHandler handler;
        handler.run();
    }
    catch(const std::logic_error&)
    {
        SUCCEED();
        return;
    }
    FAIL() << "run() did not throw logic_error exception.";
}
