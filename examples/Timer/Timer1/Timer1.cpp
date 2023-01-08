
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
        timer3.run(250ms);
        std::this_thread::sleep_for(1100ms);
        timer3.stop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    Timer<steady> timer4;
    try
    {
        timer4.tick += [](Timer<steady>&, const TimerEventArgs<steady>&) {
            std::cout << "tickle\n";
        };
        timer4.run(100ms, 2);
        std::this_thread::sleep_for(250ms);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}