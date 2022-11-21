#include "StopWatch.h"
#include <gtest/gtest.h>
#import <thread>

using namespace jimo::timing;
using namespace std::chrono_literals;

TEST(StopWatchTests, TestStopWatch)
{
    StopWatch watch;
    watch.Start();
    std::this_thread::sleep_for(200ms);
    watch.Stop();
    std::chrono::duration duration = watch.GetDuration();
    ASSERT_GE(duration, 200'000'000ns);
    ASSERT_GE(duration, 200ms);
}

TEST(StopWatchTests, TestStartAlreadyRunningWatch)
{
    StopWatch watch;
    watch.Start();
    try
    {
        watch.Start();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), "Attempting to start a stop watch that is already running!");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestStopStopWatchThatIsNotRunning)
{
    StopWatch watch;
    try
    {
        watch.Stop();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), "Attempting to stop a StopWatch that is not running.");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestStopAlreadyStopedStopWatch)
{
    StopWatch watch;
    watch.Start();
    watch.Stop();
    try
    {
        watch.Stop();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), "Attempting to stop a StopWatch that is not running.");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestGetDurationFromRunningStopWatch)
{
    StopWatch watch;
    watch.Start();
    try
    {
        [[maybe_unused]] auto duration = watch.GetDuration();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve duration from StopWatch that is currently running");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestGetDurationFromStopWatchthatHasNotRun)
{
    StopWatch watch;
    try
    {
        [[maybe_unused]] auto duration = watch.GetDuration();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve duration from StopWatch that has not been run");
        return;
    }
    FAIL();
}