
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
        timer2.tick += [](Timer<utc>&, const TimerEventArgs<utc>&) {
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
        timer3.tick += [](Timer<steady>&, const TimerEventArgs<steady>&) {
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
        timer4.tick += [](Timer<>&, const TimerEventArgs<>&) {
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
        timer5.tick += [](Timer<steady>&, const TimerEventArgs<>&) {
            std::cout << "tick, tock\n";
        };
        // fire timer 3 times at 100ms intervals starting at specified time
        timer5.run(steady::now() + 250ms, 100ms, 3);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}