#include "StopWatch.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace jimo::timing;
using namespace std::chrono_literals;

int main()
{
    try
    {
        StopWatch<std::chrono::steady_clock> watch;
        watch.start();
        // do something that you want to time
        std::this_thread::sleep_for(500us);
        watch.stop();
        auto duration = watch.getDuration();
        std::cout << "Task took " << duration.count() << "ns\n";

        watch.start();
        // do something that you want to time laps for
        for (int i = 0; i < 10; ++i)
        {
            std::this_thread::sleep_for(200us);
            watch.startNextLap();
        }
        watch.stopWithoutSavingTime();
        auto durations = watch.getLapTimes();
        std::cout << "Here are the " << durations.size() << " lap times:\n";
        for (auto dur : durations)
        {
            auto lapTime = std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
            std::cout << "    " << lapTime << "us\n";
        }
        // this throws exception
        watch.stop();
    }
    catch (StopWatchException& e)
    {
        std::cout << e.what() << std::endl;
    }
}