/// @file ActionTests.cpp
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include "Action.h"

using namespace jimo;
using namespace jimo::threading;

TEST(ActionTests, testNoArgsConstructor)
{
    enum class Actions
    {
        first,
        last,
    };
    int value {};
    Action<Actions> action;
    action.action = Actions::first;
    action.actionData = 4;
    action.actionCallback += [&value](std::any val){ value = std::any_cast<int>(val); };
    action.actionCallback(3);
    ASSERT_EQ(3, value);
}