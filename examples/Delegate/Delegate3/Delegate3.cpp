#include "Delegate.h"
#include <iostream>

using namespace jimo;

// Declare ProcessTwoNumbers Delegate
using ProcessTwoNumbers = Delegate<void, int, double>;

class MathClass
{
    public:
        void multiplyNumbers(int x, double y)
        {
            std::cout << x * y;
        }
};

int main()
{
    MathClass mathClass;
    ProcessTwoNumbers delegate { mathClass, &MathClass::multiplyNumbers };
    // Invoke Delegate object
    std::cout << "Invoking the delegate using 'multiplyNumbers':\n";
    for (int i = 1; i <= 5; ++i)
    {
        delegate(i, 3);
        std::cout << ' ';
    }
    std::cout << '\n';
}

/* Output:
 Invoking the delegate using 'multiplyNumbers':
 3 6 9 12 15
 */