#include <iostream>
#include "Delegate.h"

using namespace jimo;

using Functions = Delegate<void>;

void func()
{
    std::cout << "A message from func\n";
}

class Functor
{
    public:
        void operator ()() { std::cout << "A message from a functor\n";}
};

class AClass
{
    public:
        static void staticMethod() { std::cout << "A message from a static method\n"; }
        void instanceMethod() { std::cout << "A message from an instance method\n"; }
};

int main()
{
    Functor functor;
    AClass aClass;
    Functions functions { func };
    Functions functions2 { aClass, &AClass::instanceMethod };
    functions += functor;
    functions += AClass::staticMethod;
    functions += functions2;
    functions += []() { std::cout << "A message from a lambda\n"; };

    functions();
}

/* Output:
 A message from func
 A message from a functor
 A message from a static method
 A message from an instance method
 A message from a lambda
 */