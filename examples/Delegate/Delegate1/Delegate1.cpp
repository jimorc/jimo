#include "Delegate.h"
#include <iostream>

struct Data
{
    int a = 0;
    int b = 0;
    int c = 0;
    float d = 0.f;
    void print()
    {
        std::cout << "Data: a = " << a << " b = " << b << " c = " << c << " d = " << d << '\n';
    }
};

class Object
{
    public:
        void setA(Data& d, int x, float) { d.a = x; }
};

class AddTwoFunctor
{
    public:
        void operator ()(Data& d, int x, float ) { d.b = x + 2; }
};

void setD(Data& d, int , float f) { d.d = f; }

int main()
{
    Data data;
    Object object;
    AddTwoFunctor addTwo;
    using DataFunctions = jimo::Delegate<void, Data&, int, float>;

    // equivalent to std::bind(&Object::setA, &object, std::placeholders::_1,
    //     std::placeholders::_2, std::placeholders::_3);
    DataFunctions dataFunctions = { object, &Object::setA };
    // add a functor
    dataFunctions += addTwo;
    // add a function
    dataFunctions += setD;
    // invoke the delegate
    dataFunctions(data, 3, 16.5f);
    data.print();

    // add a lambda
    dataFunctions += [](Data& data, int x, float) -> void { data.c = x; };
    // remove the functor
    dataFunctions -= addTwo;
    dataFunctions.invoke(data, 6, -1.3f);
    data.print();
}