
#include <chrono>
#include <iostream>
#include "Timer.h"

using namespace jimo::timing;
using utc = std::chrono::utc_clock;
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
    
}