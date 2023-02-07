#include <iostream>
#include <thread>
#include <chrono>
#include "Action.h"
#include "ActionHandler.h"

using namespace jimo::threading;
using namespace std::chrono_literals;
using namespace std::placeholders;

enum class Actions
{
    terminate,
    runContinuous,
    stopContinuous,
    printMessage,
    wait,
};
void terminated(std::any)
{
    std::cout << "Terminated the handler\n";
}
void stopped(std::any)
{
    std::cout << "Stopped running continuously\n";
}
void started(std::any)
{
    std::cout << "Start running continuously\n";
}
void printIt(Action<Actions> action)
{
    auto value = std::any_cast<int>(action.actionData);
    std::cout << "In printIt. Value is " << value << '\n';
}
class Handler : public ActionHandler<Actions>
{
    public:
        Handler() : ActionHandler<Actions>() {};
        void letsWait(Action<Actions> action)
        {
            auto wTime = std::any_cast<std::chrono::milliseconds>(action.actionData);
            std::this_thread::sleep_for(wTime);
            std::cout << "Waited for " << wTime << "\n";
        }
    protected:
        void continuouslyRun() override
        {
            std::this_thread::sleep_for(50ms);
            ++count;
            if (count == 14)
            {
                std::cout << "count = 14. Will now queue a wait for 250ms\n";
                Action<Actions> wait(Actions::wait, 250ms);
                queueAction(wait);
            }
            if (count % 5 == 0)
            {
                std::cout << "count = " << count << '\n';
            }
            if (count >= 25)
            {
                Action<Actions> stop(Actions::stopContinuous, {}, 
                    [](std::any val){ stopped(val); });
                queueAction(stop);
                Action<Actions> terminate(Actions::terminate, {}, [](std::any val)
                    { terminated(val); });
                queueAction(terminate);
            }
        }
    private:
        int count {};
};

int main()
{
    Handler handler;
    handler.addHandlers({ { Actions::printMessage, printIt },
        { Actions::wait, std::bind(&Handler::letsWait, &handler, _1) } });
    handler.queueAction(Action<Actions>(Actions::printMessage, 2));
    handler.queueAction(Action<Actions>(Actions::printMessage, 3));
    Action<Actions> start(Actions::runContinuous, {}, started);
    handler.queueAction(start);
    handler.queueAction(Action<Actions>(Actions::printMessage, 6));
    handler.queueAction(Action<Actions>(Actions::wait, 100ms));
    handler.run();
    std::cout << "Returned from run()\n";
}
/*
Output:
In printIt. Value is 2
In printIt. Value is 3
Start running continuously
In printIt. Value is 6
Waited for 100ms
count = 5
count = 10
count = 14. Will now queue a wait for 250ms
Waited for 250ms
count = 15
count = 20
count = 25
Stopped running continuously
Terminated the handler
Returned from run()
*/