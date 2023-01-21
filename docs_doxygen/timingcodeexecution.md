# How to Time Code Execution
## Overview
The `jimo::timing::StopWatch` class is provided to time code execution. This class
provides the following functionality:
* Time a single run of code.
* Time *laps*, that is, time each execution within a loop.

## Timing Code Execution
Let's say you have some code that you want to time the execution of. Here is an example:
```
int main()
{
    // do some setup
    // time the execution of this function
    for (int i = 0; i < 10; ++i)
    {
        int result = timeThisFunction()
        // do something with result
    }
}
```
### Timing Entire for loop Using the Standard Templates Library
Here is how to time the entire for loop using STL classes:
```
int main()
{
    // do some setup
    // time the execution of this function
    std::chrono::time_point<std::chrono::steady_clock> startTime = 
        std::chrono::steady_clock.now();
    for (int i = 0; i < 10; ++i)
    {
        int result = timeThisFunction()
        // do something with result
    }
    std::chrono::time_point<std::chrono::steady_clock> endTime = 
        std::chrono::steady_clock.now();
    std::chrono::nanoseconds totalTime = endTime - startTime;
    std::cout << "Total elapsed time = totalTime.count() << "nsd\n";
}
```
This is relatively straight-forward; it requires four calls to the std::chrono library.
### Timing Entire for loop Using StopWatch
Here is the same functionality using the jimo::timing::StopWatch class:
```
int main()
{
    // do some setup
    // time the execution of this function
    jimo::timing::StopWatch<> watch;
    watch.start();
    for (int i = 0; i < 10; ++i)
    {
        int result = timeThisFunction()
        // do something with result
    }
    watch.stop();
    auto totalTime = watch.getDuration();
    std::cout << "Total elapsed time = totalTime.count() << "nsd\n";
}
```
As you can see, this code is a bit cleaner, but there is no great advantage to using
the StopWatch class.
### Timing Each Execution of for loop Using Standard Templates Library
Here is code to time each execution:
```
int main()
{
    // do some setup
    // time the execution of this function
    std::vector<std::chrono::nanoseconds> executionEndTimes;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 0)
        {
            std::chrono::time_point<std::chrono::steady_clock> startTime = 
                std::chrono::steady_clock.now();
        }
        int result = timeThisFunction();
        // do something with result
        executionEndTimes.emplace_back(std::chrono::steady_clock::now());
    }
    for (int timeIndex = 0, timeIndex < 10; ++timeIndex)
    {
        std::chrono::nanoseconds lapStartTime = (timeIndex == 0) ? startTime : 
            executionTimes[timeIndex - 1];
        std::chrono::nanoseconds executionTime = executionEndTimes[timeIndex] - lapStartTime;
        std::cout << "Lap " << timeIndex + 1 <<>> " elapsed time = ";
        std::cout << executionTime.count() << "nsd\n";
    }
}
```
### Timing Each Execution of for loop Using StopWatch
Here is the same functionality using jimo::timing::StopWatch:
```
int main()
{
    // do some setup
    // time the execution of this function
    jimo::timing::StopWatch<> watch;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 0)
        {
            watch.start();
        }
        int result = timeThisFunction();
        // do something with result
        watch.startNextLap();
    }
    watch.stopWithoutSavingTime();
    auto durations = watch.getLapTimes();
    auto timeIndex = 0;
    for (auto& duration : durations)
    {
        std::cout << "Lap " << timeIndex++ + 1 << " elapsed time = ";
        std::cout << duraton.count() << "nsd\n";
    }
}
```
As you can see, this is cleaner code.
