#include "Timer.h"
#include "TimerException.h"
#include "StopWatch.h"
#include <iostream>
#include <chrono>

using namespace jimo::timing;
using namespace std::chrono_literals;

int main()
{
    OnceTimer<std::chrono::steady_clock> timer([] () { std::cout << "tock\n"; });
    OnceTimer<std::chrono::steady_clock> timer2([] () { std::cout << "tick\n"; });
    OnceTimer<std::chrono::utc_clock> timer3([]() { std::cout << "tickle\n"; });
    try
    {
        // throw TimerException (timer never started)
        timer.Stop();
    }
    catch (const TimerException& e)
    {
        std::cout << e.what() << '\n';
    }
    try
    {
        // fire timer, no exception thrown
        timer2.Fire(std::chrono::steady_clock::now() +500ms);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    try
    {
        timer3.Fire(std::chrono::utc_clock::now() + 1s);
        // throw TimerException (OnceTimer can only be used once)
        // we need to wait for timer3 to fire event
        std::this_thread::sleep_for(2s);
        timer3.Fire(std::chrono::utc_clock::now() + 1s);
    }
    catch (const TimerException& e)
    {
        std::cout << e.what() << '\n';
    }
    
}