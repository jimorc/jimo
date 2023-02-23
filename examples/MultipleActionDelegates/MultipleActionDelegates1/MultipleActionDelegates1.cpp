#include "MultipleActionDelegates.h"
#include <iostream>
#include <format>

using namespace jimo::threading;

enum class Actions
{
    divideByTwo,
    divideByThree,
    divideByFive,
};

auto divideByTwo = [](int value) 
    { std::cout << std::format("{} is divisible by 2.\n", value); };
auto divideByThree = [](int value) 
    { std::cout << std::format("{} is divisible by 3.\n", value); };
auto divideByFive = [](int value) 
    { std::cout << std::format("{} is divisible by 5.\n", value); };
auto anotherByFive = [](int value) 
    { std::cout << std::format("Yes, I am confirming that {} is divisible by 5.\n", value); };
int main()
{
    MultipleActionDelegates<Actions, void, int> delegates;
    delegates.addToDelegates(Actions::divideByTwo, divideByTwo);
    delegates.addToDelegates(Actions::divideByThree, divideByThree);
    jimo::Delegate<void, int> byFive(divideByFive);
    byFive += anotherByFive;
    delegates.addToDelegates(Actions::divideByFive, byFive);

    for (int i = 1; i < 11; ++i)
    {
        if (i % 2 == 0)
        {
            delegates[Actions::divideByTwo](i);
        }
        if (i % 3 == 0)
        {
            delegates[Actions::divideByThree](i);
        }
        if (i % 5 == 0)
        {
            delegates[Actions::divideByFive](i);
        }
        if (i == 5)
        {
            delegates.removeFromDelegates(Actions::divideByFive, anotherByFive);
        }
    }
}