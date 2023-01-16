#include "Event.h"
#include "EventArgs.h"
#include "Object.h"
#include <functional>
#include <iostream>

using namespace jimo;
using namespace std::placeholders;

class NumberEventArgs : public EventArgs
{
    public:
        NumberEventArgs(int value) : m_value(value) {}
        virtual ~NumberEventArgs() {}
        auto value() const noexcept -> int { return m_value; } 
    private:
        int m_value;
};
class ObjectWithEvents : public Object
{
    public:
        ObjectWithEvents() {}
        ~ObjectWithEvents() {}
        auto value1(int value) noexcept -> void { m_value1 = value; }
        auto value2(int value) noexcept -> void { m_value2 = value; }
        auto value3(int value) noexcept -> void { m_value3 = value; }
        auto value4(int value) noexcept -> void { m_value4 = value; }
        auto value1() const noexcept -> int { return m_value1; }
        auto value2() const noexcept -> int { return m_value2; }
        auto value3() const noexcept -> int { return m_value3; }
        auto value4() const noexcept -> int { return m_value4; }
        auto setValue1(ObjectWithEvents&, const NumberEventArgs& e) noexcept
        {
            value1(e.value() - 3);
        }
        auto onEvent1(NumberEventArgs& e) noexcept -> void
        {
            std::invoke(event1, *this, e);
        }
        auto onEvent2(EventArgs& e) noexcept -> void
        {
            event2.invoke(*this, e);
        }
        auto print() const noexcept -> void
        {
            std::cout << "ObjectWithEvents: " << m_value1 << ", " << m_value2;
            std::cout << ", " << m_value3 << ", " << m_value4 << '\n';
        }
        Event<ObjectWithEvents, NumberEventArgs> event1;
        Event<Object, EventArgs> event2;
    private:
        int m_value1 {0};
        int m_value2 {0};
        int m_value3 {0};
        int m_value4 {0};
};
class AnotherObject
{
    public:
        auto setValue4(Object& sender, NumberEventArgs& e) noexcept
        {
            dynamic_cast<ObjectWithEvents&>(sender).value4(e.value() + 12);
        }
};
void func(ObjectWithEvents& sender, const NumberEventArgs& e)
{
    sender.value3(e.value());
}
class Functor
{
    public:
        auto operator ()(ObjectWithEvents& sender, NumberEventArgs& e) -> void
        {
            sender.value2(e.value() * 2);
            // halt further processing
            e.halt(true);
        }
};
int main()
{
    ObjectWithEvents owe;
    Functor ftor;
    AnotherObject anObject;
    owe.event1 += func;
    // note that ftor halts calls of delegate functions after this one
    owe.event1 += ftor;
    owe.event1 += { owe, &ObjectWithEvents::setValue1 };
    owe.event1 += { anObject, &AnotherObject::setValue4 };
    owe.event2 += [](Object&, const EventArgs&) { std::cout << "From running event2\n"; };
    std::cout << "Before events are invoked:\n";
    owe.print();

    EventArgs args;
    NumberEventArgs nea(4);
    owe.onEvent2(args);
    owe.onEvent1(nea);
    std::cout << "Only the second and third values are set because event processing";
    std::cout << " is halted after the second value is set:\n";
    owe.print();
    // remove setting of second value
    // this also removed halt, so ObjectWithEvents::setValue1 and AnotherObject::setValue4
    // will now be called.
    owe.event1 -= ftor;
    EventArgs e2;
    owe.onEvent2(e2);
    // The second value will not be change
    NumberEventArgs nea2(7);
    owe.onEvent1(nea2);   
    std::cout << "The second value will not be changed after ftor is";
    std::cout << " removed from event1\n";
    owe.print();
}