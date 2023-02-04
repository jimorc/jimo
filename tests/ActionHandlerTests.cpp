#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "ActionHandler.h"

using namespace jimo::threading;
using namespace std::placeholders;
using namespace std::chrono_literals;

enum class Actions
{
    none,
    runContinuous,
    stopContinuous,
    terminate,
    incrementValue,
    incrementAgain,
    nonAction,
};

TEST(ActionHandlerTests, testRunStopTerminate)
{
    class TestActionHandler : public ActionHandler<Actions>
    {
        public:
            TestActionHandler() : ActionHandler<Actions>() {}
            int value() { return m_value; }
        protected:
            void continuouslyRun() override
            {
                std::this_thread::sleep_for(50ms);
                if(++m_value >= 5)
                {
                    Action<Actions> stop(Actions::stopContinuous, {});
                    queueAction(stop);
                    Action<Actions> terminate(Actions::terminate, {});
                    queueAction(terminate);
                }
            }
        private:
            int m_value {};
    };
    TestActionHandler handler;
    Action<Actions> start(Actions::runContinuous, {});
    handler.queueAction(start);
    handler.run();

    ASSERT_GE(5, handler.value());
}

TEST(ActionHandlerTests, testActions)
{
    int value {};
    auto incrementValue = [&value](Action<Actions>&) { ++value; };

    class TestActionHandler : public ActionHandler<Actions>
    {
        public:
            TestActionHandler() : ActionHandler<Actions>() {};
        protected:
            void continuouslyRun() override 
            {
                throw std::logic_error("The action `runContinuous` is invalid.");
            }
    };

    TestActionHandler handler;
    handler.addHandlers({
        { Actions::incrementValue, incrementValue },
        { Actions::incrementAgain, incrementValue }
    });

    Action<Actions> incrementVal(Actions::incrementValue, {});
    Action<Actions> incrementAgain(Actions::incrementAgain, {});
    handler.queueAction(incrementVal);
    handler.queueAction(incrementVal);
    handler.queueAction(incrementVal);
    handler.queueAction(incrementVal);
    handler.queueAction(incrementVal);
    handler.queueAction(incrementAgain);
    handler.queueAction(incrementAgain);
    handler.queueAction(incrementAgain);
    handler.queueAction(incrementAgain);
    handler.queueAction(incrementAgain);
    Action<Actions> terminate(Actions::terminate, {});
    handler.queueAction(terminate);
    handler.run();

    ASSERT_EQ(10, value);
}

TEST(ActionHandlerTests, testOutOfRangeAction)
{
    int value {};
    auto incrementValue = [&value](Action<Actions>&) { ++value; };

    class TestActionHandler : public ActionHandler<Actions>
    {
        public:
            TestActionHandler() : ActionHandler<Actions>() {};
        protected:
            void continuouslyRun() override
            {
                throw std::logic_error("The action `runContinuous` is invalid.");
            }
    };

    TestActionHandler handler;
    handler.addHandlers({
        { Actions::incrementValue, incrementValue },
        { Actions::incrementAgain, incrementValue }
    });

    Action<Actions> bad(Actions::nonAction, {});
    handler.queueAction(bad);
    Action<Actions> terminate(Actions::terminate, {});
    handler.queueAction(bad);
    handler.queueAction(terminate);
    try
    {
        handler.run();
    } catch (std::out_of_range& e)
    {
        ASSERT_STREQ("The Action with enumeration value 6 has no handler.", e.what());
        return;
    }
    FAIL() << "Failed to throw out_of_range exception.";
}

TEST(ActionHandlerTests, testBadRunContinuous)
{
    int value {};
    auto incrementValue = [&value](Action<Actions>&) { ++value; };

    class TestActionHandler : public ActionHandler<Actions>
    {
        public:
            TestActionHandler() : ActionHandler<Actions>() {};
        protected:
            void continuouslyRun() override
            {
                throw std::logic_error("The action `runContinuous` is invalid.");
            }
    };

    TestActionHandler handler;
    Action<Actions> continuous(Actions::runContinuous, {});
    handler.queueAction(continuous);
    try
    {
        handler.run();
    } catch (std::logic_error& e)
    {
        ASSERT_STREQ("The action `runContinuous` is invalid.", e.what());
        return;
    }
    FAIL() << "Failed to throw logic_error exception.";
}