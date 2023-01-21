#include "StopWatch.h"
#include <gtest/gtest.h>
#include <thread>

using namespace jimo::timing;
using namespace std::chrono_literals;

TEST(StopWatchTests, TestStopWatch)
{
    StopWatch watch;
    watch.start();
    std::this_thread::sleep_for(200ms);
    watch.stop();
    std::chrono::duration duration = watch.getDuration();
    ASSERT_GE(duration, 200'000'000ns);
    ASSERT_GE(duration, 200ms);
}

TEST(StopWatchTests, TestStartAlreadyRunningWatch)
{
    StopWatch watch;
    watch.start();
    try
    {
        watch.start();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), "Attempting to start a StopWatch that is already running!");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestStopStopWatchThatIsNotRunning)
{
    StopWatch watch;
    try
    {
        watch.stop();
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
    watch.start();
    watch.stop();
    try
    {
        watch.stop();
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
    watch.start();
    try
    {
        [[maybe_unused]] auto duration = watch.getDuration();
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
        [[maybe_unused]] auto duration = watch.getDuration();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve duration from StopWatch that has not been run");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestStartNextLap)
{
    StopWatch watch;
    watch.start();
    std::this_thread::sleep_for(5ms);
    watch.startNextLap();
    std::this_thread::sleep_for(10ms);
    watch.stop();

    auto lapTimes = watch.getLapTimes();

    ASSERT_EQ(lapTimes.size(), 2);
    ASSERT_GT(lapTimes[0], 5ms);
    ASSERT_LT(lapTimes[0], 2000ms);
    ASSERT_GT(lapTimes[1], 10ms);
    ASSERT_LT(lapTimes[1], 200ms);
    auto totalLapTimes = lapTimes[0] + lapTimes[1];
    ASSERT_EQ(totalLapTimes, watch.getDuration());
}

TEST(StopWatchTests, TestGetLapTimesNoStartNextLap)
{
    StopWatch watch;
    watch.start();
    std::this_thread::sleep_for(10ms);
    watch.stop();

    auto lapTimes = watch.getLapTimes();

    ASSERT_EQ(lapTimes.size(), 1);
    ASSERT_EQ(lapTimes[0], watch.getDuration());
}

TEST(StopWatchTests, TestStartNextLapWhenStopWatchNotStarted)
{
    StopWatch watch;
    try
    {
        watch.startNextLap();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot call startNextLap for stop watch that is not running.");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestGetLapTimesWhenStopWatchIsRunning)
{
    StopWatch watch;
    watch.start();
    try
    {
        auto lapTimes = watch.getLapTimes();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve lap times while StopWatch is running");
        return;
    }
    catch (std::exception&)
    {
        FAIL();
    }
    FAIL();
}

TEST(StopWatchTests, TestGetLapTimesWhenStopWatchNeverRun)
{
    StopWatch watch;
    try
    {
        auto lapTimes = watch.getLapTimes();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve lap times. StopWatch never ran");
        return;
    }
    catch (std::exception& )
    {
        FAIL();
    }
    FAIL();
}

TEST(StopWatchTests, TestStartTwice)
{
    StopWatch watch;
    watch.start();
    watch.stop();

    EXPECT_EQ(1, watch.getLapTimes().size());

    watch.start();
    watch.stop();
    ASSERT_EQ(1, watch.getLapTimes().size());
}

TEST(StopWatchTests, TestStopWithoutSavingTime)
{
    StopWatch watch;
    watch.start();
    watch.startNextLap();
    watch.stopWithoutSavingTime();
    ASSERT_EQ(1, watch.getLapTimes().size());
}

TEST(StopWatchTests, TestStopWithoutSavingTimeBeforeStart)
{
    StopWatch watch;
    try
    {
       watch.stopWithoutSavingTime();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "stopWithoutSavingTime() being called before stop watch started");
        return;
    }
    catch (std::exception&)
    {
        FAIL();
    }
    FAIL();
}
