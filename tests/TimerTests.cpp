/// @file DelegateTests.cpp
/// @author Jim Orcheson
/// @copyright 2023 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include "Timer.h"
#include "TimerEventArgs.h"

using namespace jimo::timing;

TEST(TimerTests, TestStopBeforeRun)
{
    Timer<std::chrono::steady_clock> timer;
    try
    {
        timer.stop();
    }
    catch(const TimerException& e)
    {
        std::cout << e.what() << std::endl;
        ASSERT_STREQ("Timer::stop called for a timer that was never started.", e.what());
        return;
    }
    FAIL() << "Timer::stop should have thrown TimerException.";
}

TEST(TimerTests, TestRunOnceNow)
{
    using utc = std::chrono::utc_clock;
    utc::time_point timePoint = utc::time_point::min();
    Timer<utc> timer;
    timer.tick += [&timePoint](Timer<utc>&, const TimerEventArgs<utc>& e) {
        std::cout << e.time() << '\n';
        timePoint = e.time();
    };
    timer.run(std::chrono::utc_clock::now());
    // When tick runs, timePoint updates. Wait for it.
    while (timePoint == utc::time_point::min()) {
        std::this_thread::sleep_for(1ms);
    }
    auto duration = utc::now() - timePoint;
    ASSERT_TRUE(duration >= 0ns);
    ASSERT_TRUE(duration <= 100ms);
}

TEST(TimerTests, TestRunOnceBeforeNow)
{
    using utc = std::chrono::utc_clock;
    utc::time_point timePoint = utc::time_point::min();
    Timer<utc> timer;
    timer.tick += [&timePoint](Timer<utc>&, const TimerEventArgs<utc>& e) {
        std::cout << e.time() << '\n';
        timePoint = e.time();
    };
    timer.run(std::chrono::utc_clock::now() - 1s);
    // When tick runs, timePoint updates. Wait for it.
    while (timePoint == utc::time_point::min()) {
        std::this_thread::sleep_for(1ms);
    }
    auto duration = utc::now() - timePoint;
    ASSERT_TRUE(duration >= 0ns);
    ASSERT_TRUE(duration <= 100ms);
}

TEST(TimerTests, TestRunOnceLater)
{
    using utc = std::chrono::utc_clock;
    utc::time_point timePoint = utc::time_point::min();
    Timer<utc> timer;
    timer.tick += [&timePoint](Timer<utc>&, const TimerEventArgs<utc>& e) {
        std::cout << e.time() << '\n';
        timePoint = e.time();
    };
    timer.run(std::chrono::utc_clock::now() + 50ms);
    // When tick runs, timePoint updates. Wait for it.
    while (timePoint == utc::time_point::min()) {
        std::this_thread::sleep_for(1ms);
    }
    auto duration = utc::now() - timePoint;
    ASSERT_TRUE(duration >= 0ns);
    ASSERT_TRUE(duration <= 100ms);
}
