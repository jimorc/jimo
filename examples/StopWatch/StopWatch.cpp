#include "StopWatch.h"
#import <iostream>
#import <thread>
#import <chrono>

using namespace jimo::timing;
using namespace std::chrono_literals;

int main()
{
    StopWatch watch;
    watch.Start();
    // do something that you want to time
    std::this_thread::sleep_for(500us);
    watch.Stop();
    auto duration = watch.GetDuration();
    std::cout << "Task took " << duration.count() << "ns\n";

    watch.Start();
    // do something that you want to time laps for
    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(200us);
        watch.StartNextLap();
    }
    watch.StopWithoutSavingTime();
    auto durations = watch.GetLapTimes();
    std::cout << "Here are the " << durations.size() << " lap times:\n";
    for (auto dur : durations)
    {
        auto lapTime = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
        std::cout << "    " << lapTime << "us\n";
    }
}