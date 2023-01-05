# Events
## Overview
Events enable a class or object to notify other classes and objects when something of
interest occurs. The class that sends (raises) the event is called the publisher and the
classes that receive (or handle) the event are called subscribers. In a typical 
GUI application,
you subscribe to events raised by controls such as buttons and combo boxes. The jimo library
provides a few classes that raise events, such as Timer.

Events have the following properties:
* The publisher determines when an event is raised; the subscribers determine what action is
taken in response to the event.
* An event can have multiple subscribers. A subscriber can handle multiple events from
multiple publishers.
* Events with no subscribers are never raised.
* Events are typically used to signal user actions such as Timer ticks.
* When an event has multiple subscribers, the event handlers are invoked synchronously when
that event is raised.
* In the jimo library, events are based on the jimo::EventHandler Delegate and the 
jimo::EventArgs base class.

## How to Subscribe to and Unsubscribe from Events
### To subscribe to Events Programmatically
1. Define an event handler method whose signature matches the Delegate signature for the
event. For example, for an event that is based on the jimo::EventHandler Delegate type, the
following code represents the method stub:
```
void handleCustomEvent(Object& sender, const customEventArgs& e)
{
    // Do something useful
}
```
2. Use the addition assignment operator to attach an event handler to the event. In the
following example, assume that an object named <code>publisher</code> has an event named 
<code>raiseCustomEvent</code>. Note that the *Subscriber* class needs a reference to the 
<code>publisher</code> object in order to subscribe to its events:
```
publisher.raiseCustomEvent += { *this, &Subscriber::handleCustomEvent };
```
or
```
publisher.raiseCustomEvent += CustomEventHandler(*this, &Subscriber::handleCustomEvent);
```
### To Subscribe to Events by Using a Lambda Expression
If you don't have to unsubscribe from an event later, you can use the addition assignment
operator <code>+=</code> to attach a 
[lambda expression](https://en.cppreference.com/w/cpp/language/lambda) as an event handler.
In the following example, assume that an object named <code>publisher</code> has an event 
named <code>raiseCustomEvent</code> and that a <code>CustomEventArgs</code> class has also 
been defined to carry some
kind of specialized event information. Note that a <code>subscriber</code> object needs a 
reference to <code>publisher</code> in order to subscribe to its events.
```
publisher.raiseCustomEvent += [](Object& sender, const CustomEventArgs& e) {
    std::string s = sender.toString() + " " + e.toString();
    std::cout << s << std::endl; }
```
You cannot easily unsubscribe from an event if you used a lambda expression to subscribe to
it. To unsubscribe in this scenario, go back to the code where you subscribe to the event,
store the anonymous function in a Delegate variable, and then add the Delegate to the event.
It is recommended that you do not use a lambda expression to subscribe to events if you
have to unsubscribe from the event at some later point in your code. For more information
about anonymous functions, see 
[lambda expressions](https://en.cppreference.com/w/cpp/language/lambda).

### Unsubscribing
To prevent your event handler from being invoked when the event is raised, unsubscribe from
the event. In order to prevent a crash, you should unsubscribe from events before you
destroy a subscriber object. Until you unsubscribe from an event, the multicast Delegate
that underlies the event in the publishing object has a reference to the Delegate that
encapsulates the subscriber's event handler.
### To Unsubscribe from an Event
Use the subtraction assignment operator <code>-=</code> to unsubscribe from an event. Assuming you 
have used the code from the section 
*How to Subscribe to and Unsubscribe from Events*
to subscribe to <code>raiseCustomEvent</code>, you can unsubscribe from the event as
 follows:
```
publisher.raiseCustomEvent -= { *this, &Subscriber::handleCustomEvent };
```
or
```
publisher.raiseCustomEvent -= CustomEventHandler(*this, &Subscriber::handleCustomEvent);
```
When all subscribers have unsubscribed from an event, the event instance in thea *Publisher*
class obejct is set to empty.
## How to Publish Events that Conform to jimo Guidelines
The following procedure demonstrates how to add events that follow the standard jimo
pattern to your classes and structs. All events in the jimo library are based on the
[jimo::EventHandler] Delegate, which is defined as follows:
```
template<typename sender_t, typename eventArgs_t>
using EventHandler = Delegate<void, sender_t&, const eventArgs_t&>;
```
Although events in classes that you define can be based on any valid Delegate type, even
Delegates that return a value, it is recommended that you base your events on the jimo
pattern by using jimo::EventHandler, as shown in the following example. The name
*EventHandler* can lead to a bit of confusion as it doesn't actually handle the event.
The jimo::EventHandler type is a Delegate type. A method or lambda expression whose
signature matches the Delegate definition is the event handler and will be invoked when the
event is raised.
### Publish Events Based on the EventHandler Pattern
1. (Go to step 3a if you do not have custom data for your event.) Declare the class for
your custom data at a scope that is visible to both your *Publisher* and *Subscriber*
classes. Then add the required members to hold your custom event data. In this example, a simple integer value is returned:
```
class CustomEventArgs : jimo::EventArgs
{
    public:
        CustomEventArgs() = default;
        CustomEventArgs(int value) : m_value(value) {}
        int value() const noexcept { return m_value; }
    private:
        int m_value;
};
```
2. Declare a Delegate in your *Publisher* class. Give it a name that ends in *EventHandler*.
The second parameter specified your custom *EventArgs* type.
```
using CustomEventHandler = jimo::Delegate<Publisher, CustomEventArgs>;
```
3. Declare the event in your *Publisher* class by using one of the following steps:

a. If you have no custom EventArgs class, your event type will be the generic *EventHandler*
Delegate. You do not have to declare the Delegate because it is already declared in the 
jimo namespace that is included when you create your project. Add the following code to your
*Publisher* class:
```
jimo::Event<Publisher, jimo::EventHandler> raiseCustomEvent;
```
where *Publisher* is the name of the class that contains the *raiseCustomEvent* event.

b. If you are using a custom class derived from jimo::EventArgs, declare your event
inside your *Publisher* class and use your Delegate from step 2 as the type.
```
jimo::Event<Publisher, CustomEventHandler> raiseCustomEvent;
```
## Example
The following example demonstrates the previous steps using both a custom EventArgs class
and a generic EventArgs class:
```
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
        Event<Publisher, CustomEventArgs> raiseCustomEvent;
        Event<Publisher, EventArgs> raiseGenericEvent;

        void doSomething()
        {
            // Write some code that does something useful here
            // then raise the event. You can also raise an event
            // before you execute a block of code.
            onRaiseCustomEvent(CustomEventArgs(++m_value));
            onRaiseGenericEvent(EventArgs());
        }
    protected:
        void onRaiseCustomEvent(const CustomEventArgs& e)
        {
            // Make a temporary copy of the event to avoid possibility of
            // a race condition if the last subscriber unsubscribes
            // immediately after the empty check and before the event
            // is raised.
            Event<Publisher, CustomEventArgs> raiseEvent = raiseCustomEvent;

            // Event will be empty if there are no subscribers.
            if (!raiseEvent.empty())
            {
                // Call to raise the event
                raiseEvent(*this, e);
            }
        }
        void onRaiseGenericEvent(const EventArgs& e)
        {
            Event<Publisher, EventArgs> raiseEvent = raiseGenericEvent;
            if (!raiseEvent.empty())
            {
                raiseEvent(*this, e);
            }
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
            publisher.raiseCustomEvent += { *this, &Subscriber::handleCustomEvent };
            publisher.raiseGenericEvent += { *this, &Subscriber::handleGenericEvent };
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

/* Output
 sub1 received this message: 1
 sub2 received this message: 1
 sub1 received generic message
 sub2 received generic message
 sub1 received this message: 2
 sub2 received this message: 2
 sub1 received generic message
 sub2 received generic message
 sub1 received this message: 3
 sub2 received this message: 3
 sub1 received generic message
 sub2 received generic message
 */
```
