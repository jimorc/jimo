#include "StopWatch.h"
#include <gtest/gtest.h>
#include <thread>

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

TEST(StopWatchTests, TestStartNextLap)
{
    StopWatch watch;
    watch.Start();
    std::this_thread::sleep_for(5ms);
    watch.StartNextLap();
    std::this_thread::sleep_for(10ms);
    watch.Stop();

    auto lapTimes = watch.GetLapTimes();

    ASSERT_EQ(lapTimes.size(), 2);
    ASSERT_GT(lapTimes[0], 5ms);
    ASSERT_LT(lapTimes[0], 2000ms);
    ASSERT_GT(lapTimes[1], 10ms);
    ASSERT_LT(lapTimes[1], 200ms);
    auto totalLapTimes = lapTimes[0] + lapTimes[1];
    ASSERT_EQ(totalLapTimes, watch.GetDuration());
}

TEST(StopWatchTests, TestGetLapTimesNoStartNextLap)
{
    StopWatch watch;
    watch.Start();
    std::this_thread::sleep_for(10ms);
    watch.Stop();

    auto lapTimes = watch.GetLapTimes();

    ASSERT_EQ(lapTimes.size(), 1);
    ASSERT_EQ(lapTimes[0], watch.GetDuration());
}

TEST(StopWatchTests, TestStartNextLapWhenStopWatchNotStarted)
{
    StopWatch watch;
    try
    {
        watch.StartNextLap();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot call StartNextLap for stop watch that is not running.");
        return;
    }
    FAIL();
}

TEST(StopWatchTests, TestGetLapTimesWhenStopWatchIsRunning)
{
    StopWatch watch;
    watch.Start();
    try
    {
        auto lapTimes = watch.GetLapTimes();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve lap times while stop watch is running");
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
        auto lapTimes = watch.GetLapTimes();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "Cannot retrieve lap times. Stop watch never ran");
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
    watch.Start();
    watch.Stop();

    EXPECT_EQ(1, watch.GetLapTimes().size());

    watch.Start();
    watch.Stop();
    ASSERT_EQ(1, watch.GetLapTimes().size());
}

TEST(StopWatchTests, TestStopWithoutSavingTime)
{
    StopWatch watch;
    watch.Start();
    watch.StartNextLap();
    watch.StopWithoutSavingTime();
    ASSERT_EQ(1, watch.GetLapTimes().size());
}

TEST(StopWatchTests, TestStopWithoutSavingTimeBeforeStart)
{
    StopWatch watch;
    try
    {
       watch.StopWithoutSavingTime();
    }
    catch (StopWatchException& e)
    {
        ASSERT_STREQ(e.what(), 
            "StopWithoutSavingTime() being called before stop watch started");
        return;
    }
    catch (std::exception&)
    {
        FAIL();
    }
    FAIL();
}
