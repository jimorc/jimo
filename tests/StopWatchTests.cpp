#include "StopWatch.h"
#include <gtest/gtest.h>
#import <thread>

using namespace jimo::timing;
using namespace std::chrono_literals;

TEST(StopWatchTests, StopWatchTest)
{
    StopWatch watch;
    watch.Start();
    std::this_thread::sleep_for(200ms);
    watch.Stop();
    std::chrono::duration duration = watch.Duration();
    ASSERT_GE(duration, 200'000'000ns);
    ASSERT_GE(duration, 200ms);
}