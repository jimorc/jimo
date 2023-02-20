/// @file MultipleActionDelegatesTests.cpp
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include "MultipleActionDelegates.h"

using namespace jimo;
using namespace jimo::threading;

enum class TestActions
{
    action1,
    action2,
    action3,
};

int value;
int value2;
auto setValue = [](std::any val) { value = std::any_cast<int>(val); };
auto timesTwo = [](std::any val) { value2 = std::any_cast<int>(val) * 2; };

TEST(MultipleActionDelegatesTests, testAddFunctionToDelegates)
{
    value = 0;
    value2 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action1, timesTwo);
    actDelegates[TestActions::action1](42);
    ASSERT_EQ(42, value);
    ASSERT_EQ(84, value2);
}

TEST(MultipleActionDelegatesTests, testAddDelegateToDelegates)
{
    value = -10;
    value2 = -1;
    MultipleActionDelegates<TestActions> actDelegates;
    Delegate<void, std::any> delegate(setValue);
    delegate += timesTwo;
    actDelegates.addToDelegates(TestActions::action1, delegate);
    actDelegates[TestActions::action1](42);
    ASSERT_EQ(42, value);
    ASSERT_EQ(84, value2);
}

TEST(MultipleActionDelegatesTests, testMulitpleActions)
{
    value = 0;
    value2 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action2, timesTwo);
    actDelegates[TestActions::action1](42);
    ASSERT_EQ(42, value);
    ASSERT_EQ(0, value2);
    actDelegates[TestActions::action2](10);
    ASSERT_EQ(42, value);
    ASSERT_EQ(20, value2);
}

TEST(MultipleActionDelegatesTests, testActionsNotDefined)
{
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action2, timesTwo);
    try
    {
        auto delegates = actDelegates[TestActions::action3];
    }
    catch(const std::out_of_range&)
    {
        SUCCEED();
        return;
    }
    FAIL() << "Test did not throw expected std::out_of_range exception";
}

TEST(MultipleActionDelegatesTests, testRemoveFunctionFromDelegates)
{
    value = 2;
    value2 = 13;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action1, timesTwo);
    actDelegates.removeFromDelegates(TestActions::action1, timesTwo);
    actDelegates[TestActions::action1](4);
    ASSERT_EQ(4, value);
    ASSERT_EQ(13, value2);
}

TEST(MultipleActionDelegatesTests, testRemoveDelegateFromDelegates)
{
    value = 3;
    value2 = 1;
    MultipleActionDelegates<TestActions> actDelegates;
    Delegate<void, std::any> delegate(setValue);
    delegate += timesTwo;
    actDelegates.addToDelegates(TestActions::action1, delegate);
    ASSERT_EQ(2, actDelegates[TestActions::action1].size());
    delegate -= timesTwo;
    Delegate<void, std::any> delegate2(setValue);
    ASSERT_TRUE(delegate == delegate2);
    actDelegates.removeFromDelegates(TestActions::action1, delegate);
    Delegate<void, std::any> delegate3(timesTwo);
    ASSERT_TRUE(actDelegates[TestActions::action1] == delegate3);
    actDelegates[TestActions::action1](7);
    ASSERT_EQ(1, actDelegates[TestActions::action1].size());
    ASSERT_EQ(3, value);
    ASSERT_EQ(14, value2);
}