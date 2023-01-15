# Delegates
## Overview
A jimo::Delegate is a thread safe type that represents references to std::functions, 
that is functions, 
static class methods, class instance methods, functors, and lambdas, with a particular
return type and parameter list. You invoke or execute the delegated functions through the
Delegate instance.

Delegates are used to pass methods as arguments to other methods. Event handlers are methods
that are invoked through Delegates. You can create a custom method and another class can
call your method when a certain event occurs. For example, your custom method can be called
for every [Timer](timer.md) tick event.

The following example shows a Delegate declaration representing functions that take two
integers as parameters, and returns an integer:
```
using PerformCalculation =  jimo::Delegate<int, int, int>;
```

Any function, static or instance class method, functor, or lambda that matches the Delegate 
type can be assigned to and removed from the Delegate object. This flexibility means that
you can programmatically change method calls, or plug new code into existing classes.

### Note
* In the context of method overloading, the signature of a method does not include the
return type. But in the context of Delegates, the signature does include the return
type. In other words, a function or method that is assigned to a Delegate object must
have the same return type as the delegate.

This ability to refer to a method as a parameter makes Delegates ideal for defining callback
functions and methods. You can write a method that compares two objects in your application.
That method can be used in a Delegate for a sort algorithm. Because the comparison code is
separate from the library, the sort method can be more general.

## Difference Between jimo::Delegate and std::function
Unlike std::functions, jimo::Delegates can be chained together; for example, mulitple
std::functions can be called on a single event.

jimo::Delegate is a collection of std::functions.

## Delegate Overview
Delegates have the following properties:
* Delegates are similar to C++ function pointers, but Delegates, like std::function, are
fully object-oriented, and unlike C++ pointers to member functions, Delegates can 
encapsulate both an object instance and a method.
* Delegates allow methods to be passed as parameters.
* Delegates can be used as callback methods.
* Unlike std::function, Delegates can be chained together; for example, multiple methods
can be called on a single event.
* Lambda expressions are a more concise way of writing inline code blocks. Lambda 
expressions in certain contexts are compiled to Delegate types. For more information about
lambda expressions, see 
[Lambda expressions](https://en.cppreference.com/w/cpp/language/lambda).

## Using Delegates
A jimo::Delegate is a type that safely encapsulates a function, functor, static or instance method, or a lambda, similar to a function pointer in C++. Unlike function pointers,
Delegates are object-oriented, type safe, and secure. The type of a Delegate is defined by
the name of the Delegate. The following example declares Delegate called LineWriter that
can encapsulate a method that takes a std::string as an argument and returns a void:
```
using LineWriter = jimo::Delegate<void, std::string>;
```

A Delegate object is normally constructed by providing the name of the method that the
Delegate will wrap, or with a lambda expression. Once a Delegate is instantiated. a method
call made to that Delegate object will be passed by the object to that method. The
parameters passed to the delegate by the caller are passed to the method, and the return
value, if any, from the method is returned to the caller by the delegate. This is known
as invoking the Delegate. An instantiated Delegate can be invoked as if it were the
wrapped method itself. Here is an example:

```
// Create a method for a Delegate.
void delegateMethod(const std::string& message)
{
    std::cout << message << std::endl;
}
```
```
// Instantiate the Delegate.
LineWriter handler { delegateMethod };
// Call the Delegate.
handler("Hello World!");
```
or
```
// Instantiate the Delegate.
LineWriter handler = LineWriter(delegateMethod);
// Call the Delegate.
handler("Hello world!");
```

Because the instantiated Delegate is an object, it can be passed as a parameter or assigned
to a property. This allows a method to accept a Delegate as a parameter, and call the
Delegate at some later time. This is known as asynchronous callback, and is a common
technique of notifying a caller where a long process has completed. When a Delegate is used
in this fashion, the code using the Delegate does not need any knowledge of the
implementation of the method being used. The functionality is similar to the encapulation
abstract classes provide.

Another common use of callbacks is defining a custom comparison method and passing that
Delegate to a sort method.

Here is an example that uses the LineWriter Delegate type as a parameter:
```
void methodWithCallback(int value1, int value2, LineWriter callback)
{
    callback("The number is: " + to_string(value 1 + value2));
}
```
You can then pass the Delegate created above to that method:
```
methodWithCallback(1, 2, handler);
```
and receive the following output on the console:
```
The number is: 3
```
Using the Delegate as an abstraction, <code>methodWithCallback</code> does not need to call
the <code>std::cout</code> directly - it does not have to be designed with 
<code>std::cout</code> in mind. What <code>methodWithCallback</code> does is simply prepare
a string and pass the string to another method. This is especially powerful since a
delegated method can use any number of parameters. When a jimo::Delegate is constructed to
wrap an instance method, the Delegate references both the instance and the method. A
Delegate has no knowledge of the instance type aside from the method it wraps, so a
Delegate can refer to any type of object as long as there is a method on that object that
matches the Delegate signature. When a Delegate is constructed to wrap a static method, it
only references the method. Consider the following declarations:
```
class AClass
{
    public:
        void method1(const std::string& message) {}
        void method2(const std::string& message) {}
};
```
Along with the static <code>delegateMethod</code> shown previously, we now have three
methods that can be wrapped in a <code>LineWriter</code> instance.

A Delegate can call more than one method when invoked. This is referred to as multicasting.
To add an extra method to the Delegate's list of methods - the invocation list - simply
requires adding two delegates using the addition assignment operator (+=). For example:
```
AClass obj;
LineWriter lr1 { obj, &AClass::method1 };
LineWriter lr2 { obj, &AClass::method2 };

LineWriter allMethodsDelegate = lr1;
allMethodsDelegate += lr2;
allMethodsDelegate += delegateMethod;
```
At this point, <code>allMethodsDelegate</code> contains three methods in its invocation
list - <code>method1</code>, <code>method2</code>, and <code>delegateMethod</code>.
The original delegates, <code>lr1</code> and <code>lr2</code>, remain unchanged. When
<code>allMethodsDelegate</code> is invoked, all three methods are called in order. If the
Delegate uses reference parameters, the reference is passed sequentially to each of the
three methods in turn, and any changes by one method are visible to the next method. When
any of the methods throws an exception that is not caught within the method, that
exception is passed to the caller of the Delegate and no subsequent methods in the
invocation list are called. If the Delegate has a return value and/or out parameters, it
returns the return value and parameters of the last method invoked. To remove a method
from the invocation list, use the subtraction assignment operator (-=). For example:
```
// Remove method1
allMethodsDelegate -= lr1;
// Remove method2
allMethodsDelegate -= { obj, &AClass::method2 };
```
Because jimo::Delegate types are classes, the methods and properties defined by the class
can be called on the Delegate. For example, to find the number of methods in a Delegate's
invocation list, call:
```
size_t invocationCount = allMethodsDelegate.size();
```
Multicast delegates are used extensively in event handling. jimo::Event source objects send
event notifications to recipient objects that have registered to receive that event. To
register for an event, the recipient creates a method designed to handle the event, then
creates a Delegate for that method and passes the Delegate to the event source. The source
calls the Delegate when the event occurs. The Delegate then calls the event handling
method on the recipient, delivering the event data. The Delegate type for a given event is
defined by the event source. For more information, see [Events](events.md).

Comparing Delegates of two different types will result in a compilation error. For example:
```
Delegate<int, int> intInt;
Delegate<void, int> voidInt;
// Compile time error
// if (intInt == voidInt) {...}

Delegate<int, int> intInt2;
// intInt and intInt2 are equal only if they contain the same methods in the same order.
```

## Delegates With Named Methods
A jimo::Delegate can be associated with a named method. When you instantiate a named method,
the method is passed as a parameter; for example:
```
// Declare a Delegate
using Worker = jimo::Delegate<void, int>;
// Define a class containing a worker method
class WorkerClass
{
    public:
        void doWork(int x) {/* do something with x*/}
}:
//Instantiate class object, and Worker using the method as parameter
WorkerClass object;
Worker worker { object, &WorkerClass::doWork };
// or alternatively
// Worker worker;
// worker += object, &WorkerClass::doWork };
```
This is called using a named method. Delegates constructed with named methods can
encapsulate either a static method or instance method. However, in a situation where
creating a new method is unwanted overhead, C++ and jimo enable you to instantiate a
Delegate and immediately specify a lambda that the Delegate will process when it is
called.

### Remarks
* The method that you pass as a Delegate parameter must have the same signature as the
Delegate declaration.
* A delegate instance may encapsulate a function, a functor, a static or instance method,
or a lambda.
* If a lambda is specified either in a Delegate constructor or Delegate::operator += call, then
the lambda cannot be removed by repeating the lambda in a Delegate::operator-= call.

### Example 1
The following is a simple example of declaring and using a Delegate. Notice that both the
Delegate, <code>ProcessTwoNumbers</code>, and the associated method, 
<code>multiplyNumbers</code> have the same signature:
@include Delegate/Delegate3/Delegate3.cpp

### Example 2
In the following example, one Delegate is mapped to a function, a functor, a static
method, an instance method, and a lambda.
@include Delegate/Delegate2/Delegate2.cpp