#include <iostream>
#include "Event.h"
#include "EventArgs.h"
#include "Object.h"

using namespace jimo;

// Declare a class to hold custom event info
class CustomEventArgs : public EventArgs
{
    public:
        CustomEventArgs() = default;
        CustomEventArgs(int value) : m_value(value) {}
        int value() const noexcept { return m_value; }
    private:
        int m_value;
};

// Class that publishes an event
class Publisher : public Object
{
    public:
        Publisher() : m_value(0) {}
        // Declare the events
        Event<Publisher, CustomEventArgs> customEvent;
        Event<Publisher, EventArgs> genericEvent;

        void doSomething()
        {
            // Write some code that does something useful here
            // then raise the event. You can also raise an event
            // before you execute a block of code.
            CustomEventArgs cea(++m_value);
            EventArgs e;
            onCustomEvent(cea);
            onGenericEvent(e);
        }
    protected:
        void onCustomEvent(CustomEventArgs& e)
        {
            customEvent(*this, e);
        }
        void onGenericEvent(EventArgs& e)
        {
            genericEvent(*this, e);
        }
    private:
        int m_value;
};

// Class that subscribes to an event
class Subscriber
{
    public:
        Subscriber(const std::string& id, Publisher& publisher) : m_id(id)
        {
            // Subscribe to the events
            publisher.customEvent += { *this, &Subscriber::handleCustomEvent };
            publisher.genericEvent += { *this, &Subscriber::handleGenericEvent };
        }

        // Define what actions to take when custom event is raised.
        void handleCustomEvent(Object&, const CustomEventArgs& e)
        {
            std::cout << m_id << " received this message: " << e.value() << '\n';
        }
        // Define what actions to take when generic event is raised.
        void handleGenericEvent(Object&, const EventArgs&)
        {
            std::cout << m_id << " received generic message\n";
        }
    private:
        std::string m_id;
};

int main()
{
    Publisher publisher;
    Subscriber sub1("sub1", publisher);
    Subscriber sub2("sub2", publisher);
    publisher.doSomething();
    publisher.doSomething();
    publisher.doSomething();
}