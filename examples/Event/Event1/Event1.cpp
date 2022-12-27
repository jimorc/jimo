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
        auto onEvent1(const NumberEventArgs& e) noexcept -> void
        {
            event1(*this, e);
        }
        auto onEvent2(const EventArgs& e) noexcept -> void
        {
            std::invoke(event2, *this, e);
        }
        auto print() const noexcept -> void
        {
            std::cout << "ObjectWithEvents: " << m_value1 << ", " << m_value2;
            std::cout << ", " << m_value3 << ", " << m_value4 << '\n';
        }
        Event<ObjectWithEvents, const NumberEventArgs> event1;
        Event<Object, const EventArgs> event2;
    private:
        int m_value1;
        int m_value2;
        int m_value3;
        int m_value4;
};
class AnotherObject : public Object
{
    public:
        auto setValue4(Object& sender, const NumberEventArgs& e) noexcept
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
        auto operator ()(ObjectWithEvents& sender, const NumberEventArgs& e) -> void
        {
            sender.value2(e.value() * 2);
        }
};
int main()
{
    ObjectWithEvents owe;
    Functor ftor;
    AnotherObject anObject;
    owe.event1 += func;
    owe.event1 += ftor;
    owe.event1 += std::bind(&ObjectWithEvents::setValue1, &owe, _1, _2);
    owe.event1 += std::bind(&AnotherObject::setValue4, &anObject, _1, _2);
    owe.event2 += [](Object&, const EventArgs&) { std::cout << "From running event2\n"; };

    owe.onEvent2(EventArgs());
    owe.onEvent1(NumberEventArgs(4));
    std::cout << "All 4 values have been set:\n";
    owe.print();
    // remove setting of second value
    owe.event1 -= ftor;
    // remove setting of fourth value
    owe.event1 -= std::bind(&AnotherObject::setValue4, &anObject, _1, _2);
    owe.onEvent2(EventArgs());
    // only first and third values will be changed
    owe.onEvent1(NumberEventArgs(7));   
    std::cout << "Only the first and third values are set after two std::functions are";
    std::cout << " removed from event1\n";
    owe.print();
}