# Timers
## Overview
Timers enable functions or other classes to perform tasks at specified times and intervals.
Timers are often provided in GUI frameworks, but the jimo library provides a Timer class
that allows the running of a timer in console applications and in GUI applications outside
of the thread that runs the GUI framework.

Timers have the following properties:
* Timers can fire `tick` event handlers once now, or at a specified time in the future.
* Timers can be fire `tick` event handlers multiple times at a specified interval.
* Timers can be stopped while running.
* Timers can fire `stopped` event handlers when the timers stop. Timers stop either when the
specified number of intervals has expired, or when the Timer::stop method is called.
* Any number of `tick` and `stopped` event handlers can be attached to a timer object.
* The event handlers execute synchronously on a different thread than the one on which the
Timer object was declared.
## How to Subscribe to and Unsubscribe from Events
### To subscribe to Events Programmatically
1. Define an event handler method whose signature matches the following:
```
void handleTickEvent(Object& sender, TimerEventArgs<clock_t>& e)
{
    // do something when tick event fires
}
```
or
```
void handleStoppedEvent(Object& sender, TimerEventArgs<clock_t>& e)
{
    // do something when stopped event fires
}
```
Note that the signature is the same for both methods.

2. Use the addition assignment operator `+=` to attach the event handler to the event. The
following code assumes that a class called `Subscriber` contains the two methods defined
above:
```
timer.tick += { *this, &Subscriber::handleTickEvent };
timer.stopped += { *this, &Subscriber::handleStoppedEvent };
```
You can subscribe functions, functors, static class methods, and lambdas as well. See Events
for more information.
## TimerEventArgs
The `jimo::timing::TimerEventArgs` class, which is derived from the `jimo::EventArgs` class,
contains the time at which the event fired. Here is some code that shows how to retrieve
the time:
```
void handleTickEvent(Object& sender, TimerEventArgs<clock_t>& e)
{
    // clock_t is the clock type that the timer was created to use.
    // e.g. std::chrono::steady_clock by default
    std::chrono::time_point<clock_t> tickTime = e.time();
    // do something with tickTime
}
```
## Timer Thread
As mentioned above, each Timer object runs on a separate thread from the thread that
declared the object, and from every other Timer object. For cross-thread communications,
see the topic *Cross-Thread Communications*.
## Example
The following program demonstrates the use of Timer objects:
```
#include <chrono>
#include <iostream>
#include "Timer.h"

using namespace jimo::timing;
using utc = std::chrono::utc_clock;
using steady = std::chrono::steady_clock;
int main() 
{
    Timer<utc> timer1;
    try
    {
        // throw TimerException because timer1 was never started.
        timer1.stop();
    }
    catch(const TimerException& e)
    {
        std::cerr << e.what() << '\n';
    }
    Timer<utc> timer2;
    try
    {
        // run timer once, now. No exception should be thrown.
        timer2.tick += [](Timer<utc>&, TimerEventArgs<utc>&) {
                std::cout << "tick\n";
            };
        timer2.run(utc::now());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    Timer<steady> timer3;
    try
    {
        timer3.tick += [](Timer<steady>&, TimerEventArgs<steady>&) {
            std::cout << "tock\n";
        };
        // run timer forever at 250ms intervals
        timer3.run(250ms);
        // let timer fire 4 times
        std::this_thread::sleep_for(1100ms);
        // and then stop the timer. Otherwise, it will run forever.
        timer3.stop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    Timer timer4;
    try
    {
        timer4.tick += [](Timer<>&, TimerEventArgs<>&) {
            std::cout << "tickle\n";
        };
        // fire timer twice at 100ms intervals
        timer4.run(100ms, 2);
        // wait for timer to finish running
        std::this_thread::sleep_for(250ms);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    Timer<steady> timer5;
    try
    {
        timer5.tick += [](Timer<steady>&, TimerEventArgs<steady>&) {
            std::cout << "tick, tock\n";
        };
        timer5.stopped += [](Timer<steady>&, TimerEventArgs<steady>& e) {
            std::cout << "Timer stopped at " << e.time().time_since_epoch().count() << '\n';
        };
        // fire timer 3 times at 100ms intervals starting at specified time
        timer5.run(steady::now() + 250ms, 100ms, 3);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
/* Output
Timer::stop called for a timer that was never started.
tick
tock
tock
tock
tock
tickle
tickle
tick, tock
tick, tock
tick, tock
Timer stopped at 3399827739791
*/
```