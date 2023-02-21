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
int value3;
auto setValue = [](std::any val) { value = std::any_cast<int>(val); };
auto timesTwo = [](std::any val) { value2 = std::any_cast<int>(val) * 2; };
auto square = [](std::any val) { int valueToSquare = std::any_cast<int>(val);
                value3 = valueToSquare * valueToSquare; };

TEST(MultipleActionDelegatesTests, testAddFunctionToDelegates)
{
    value = 0;
    value2 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue)
                .addToDelegates(TestActions::action1, timesTwo);
    actDelegates[TestActions::action1](42);
    ASSERT_EQ(42, value);
    ASSERT_EQ(84, value2);
}

TEST(MultipleActionDelegatesTests, testAddDelegateToDelegates)
{
    value = -10;
    value2 = -1;
    value3 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    Delegate<void, std::any> delegate(setValue);
    delegate += square;
    Delegate<void, std::any> delegate2(timesTwo);
    actDelegates.addToDelegates(TestActions::action1, delegate)
                .addToDelegates(TestActions::action1, delegate2);
    actDelegates[TestActions::action1](4);
    ASSERT_EQ(4, value);
    ASSERT_EQ(8, value2);
    ASSERT_EQ(16, value3);
}

TEST(MultipleActionDelegatesTests, testMulitpleActions)
{
    value = 0;
    value2 = 0;
    value3 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action2, timesTwo);
    actDelegates.addToDelegates(TestActions::action1, square);
    actDelegates[TestActions::action1](6);
    ASSERT_EQ(6, value);
    ASSERT_EQ(0, value2);
    ASSERT_EQ(36, value3);
    actDelegates[TestActions::action2](10);
    ASSERT_EQ(6, value);
    ASSERT_EQ(20, value2);
    ASSERT_EQ(36, value3);
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
    value3 = 0;
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue);
    actDelegates.addToDelegates(TestActions::action1, timesTwo);
    actDelegates.addToDelegates(TestActions::action1, square);
    actDelegates.removeFromDelegates(TestActions::action1, timesTwo)
                .removeFromDelegates(TestActions::action1, square);
    actDelegates[TestActions::action1](4);
    ASSERT_EQ(4, value);
    ASSERT_EQ(13, value2);
    ASSERT_EQ(0, value3);
}

TEST(MultipleActionDelegatesTests, testRemoveDelegateFromDelegates)
{
    value = 3;
    value2 = 1;
    value3 = 14;
    MultipleActionDelegates<TestActions> actDelegates;
    Delegate<void, std::any> delegate(setValue);
    delegate += square;
    delegate += timesTwo;
    actDelegates.addToDelegates(TestActions::action1, delegate);
    ASSERT_EQ(3, actDelegates[TestActions::action1].size());
    Delegate<void, std::any> delegate2(timesTwo);
    Delegate<void, std::any> delegate3(setValue);
    actDelegates.removeFromDelegates(TestActions::action1, delegate2)
                .removeFromDelegates(TestActions::action1, delegate3);
    ASSERT_EQ(1, actDelegates[TestActions::action1].size());
    Delegate<void, std::any> delegate4(square);
    ASSERT_TRUE(actDelegates[TestActions::action1] == delegate4);
    actDelegates[TestActions::action1](6);
    ASSERT_EQ(3, value);
    ASSERT_EQ(1, value2);
    ASSERT_EQ(36, value3);
}

TEST(MultipleActionDelegatesTests, testRemoveFunctionFromDelegatesNoDelegates)
{
    MultipleActionDelegates<TestActions> actDelegates;
    try
    {
        actDelegates.removeFromDelegates(TestActions::action2, timesTwo);
    }
    catch(const std::out_of_range&)
    {
        SUCCEED();
        return;
    }
    FAIL() << "Did not throw std::out_of_range exception";
}

TEST(MultipleActionDelegatesTests, testRemoveDelegateFromDelegatesNoDelegates)
{
    MultipleActionDelegates<TestActions> actDelegates;
    Delegate<void, std::any> delegate(timesTwo);
    try
    {
        actDelegates.removeFromDelegates(TestActions::action2, delegate);
    }
    catch(const std::out_of_range&)
    {
        SUCCEED();
        return;
    }
    FAIL() << "Did not throw std::out_of_range exception";
}

TEST(MultipleActionDelegatesTests, testClear)
{
    MultipleActionDelegates<TestActions> actDelegates;
    actDelegates.addToDelegates(TestActions::action1, setValue)
                .addToDelegates(TestActions::action1, square)
                .addToDelegates(TestActions::action2, timesTwo);
    actDelegates.clear();
    ASSERT_TRUE(actDelegates.empty());
}