/// @file ActionTests.cpp
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.

#include <vector>
#include <memory>
#include <utility>
#include <gtest/gtest.h>
#include "Action.h"

using namespace jimo;
using namespace jimo::threading;

enum class Actions
{
    first,
    last,
};

TEST(ActionTests, testNoArgsConstructor)
{
    int value{};
    Action<Actions> action;
    action.action = Actions::first;
    action.actionData = 4;
    action.actionCallback += [&value](std::any val)
    { value = std::any_cast<int>(val); };
    action.actionCallback(3);
    ASSERT_EQ(3, value);
}

TEST(ActionTests, testCopyConstructor)
{
    auto func = [](const std::any &) { /* do nothing */ };
    Action<Actions> action;
    action.action = Actions::last;
    action.actionData = 42;
    action.actionCallback += func;
    Action action2(action);
    ASSERT_EQ(action.action, action2.action);
    ASSERT_EQ(std::any_cast<int>(action.actionData), std::any_cast<int>(action2.actionData));
    ASSERT_EQ(action.actionCallback, action2.actionCallback);
}

TEST(ActionTests, testMoveConstructor)
{
    auto func = [](const std::any &) { /* do nothing */ };
    Action<Actions> action;
    action.action = Actions::last;
    action.actionData = 42;
    action.actionCallback += func;
    Action<Actions> actionCopy(action);
    Action action2(std::move(action));
    ASSERT_EQ(Actions::last, action2.action);
    ASSERT_EQ(42, std::any_cast<int>(action2.actionData));
    ASSERT_EQ(actionCopy.actionCallback, action2.actionCallback);
    ASSERT_EQ(Actions::first, action.action);
    ASSERT_FALSE(action.actionData.has_value());
    ASSERT_TRUE(action.actionCallback.empty());
}

TEST(ActionTests, testCopyEquals)
{
    auto func = [](const std::any &) { /* do nothing */ };
    Action<Actions> action;
    action.action = Actions::last;
    action.actionData = 42;
    action.actionCallback += func;
    Action<Actions> action2 = action;
    ASSERT_EQ(action.action, action2.action);
    ASSERT_EQ(std::any_cast<int>(action.actionData), std::any_cast<int>(action2.actionData));
    ASSERT_EQ(action.actionCallback, action2.actionCallback);
}

TEST(ActionTests, testMoveEquals)
{
    auto func = [](const std::any &) { /* do nothing */ };
    Action<Actions> action;
    action.action = Actions::last;
    action.actionData = 42;
    action.actionCallback += func;
    Action<Actions> actionCopy(action);
    Action<Actions> action2 = std::move(action);
    ASSERT_EQ(Actions::last, action2.action);
    ASSERT_EQ(42, std::any_cast<int>(action2.actionData));
    ASSERT_EQ(actionCopy.actionCallback, action2.actionCallback);
    ASSERT_EQ(Actions::first, action.action);
    ASSERT_FALSE(action.actionData.has_value());
    ASSERT_TRUE(action.actionCallback.empty());
}

TEST(ActionTests, testThreeArgsConstructor)
{
    int value{};
    Action<Actions> action(Actions::first, 4, [&value](std::any val)
    { value = std::any_cast<int>(val); });
    action.actionCallback(5);
    ASSERT_EQ(5, value);

    jimo::Delegate<void, std::any> callbacks;
    callbacks += [&value](std::any val)
    { ++value; };
    callbacks += [&value](std::any val)
    { value += 4; };
    Action<Actions> action2(Actions::last, 4, callbacks);
    action2.actionCallback(2);
    ASSERT_EQ(10, value);
}

TEST(ActionTests, testMethodConstructor)
{
    class Object
    {
        public:
            Object() = default;
            void cback(std::any value) { m_value = std::any_cast<int>(value); }
            int value() { return m_value; }
        private:
            int m_value {};
    };
    Object o;
    Action<Actions> action(Actions::first, {4}, o, &Object::cback);
    action.actionCallback(42);
    ASSERT_EQ(42, o.value());
}