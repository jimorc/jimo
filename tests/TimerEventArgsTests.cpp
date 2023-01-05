/// @file TimerEventArgsTests.cpp
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include "TimerEventArgs.h"

using namespace jimo::timing;

TEST(TimerEventArgsTests, TestTime)
{
    TimerEventArgs<std::chrono::steady_clock> timerEventArgs;

std::chrono::nanoseconds duration = timerEventArgs.time() - std::chrono::steady_clock::now();
    ASSERT_GE(0, duration.count());
}
