/// @file EventTests.cpp
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include "Event.h"
#include "EventHandler.h"
#include <iostream>
#include <concepts>
#include <type_traits>

using namespace jimo;

class MyObj : public Object
{
    public:

        Event<MyObj, EventArgs> anEvent;
};
void func(Object&, const EventArgs&)
{
    std::cout << "In func\n";
}
TEST(EventTests, TestConstructor)
{
    MyObj object;
    ASSERT_TRUE(object.anEvent.empty());
    object.anEvent += []( Object&, const EventArgs&) -> void { std::cout << "In event handler\n"; };
    object.anEvent(object, EventArgs());
    ASSERT_FALSE(object.anEvent.empty());
    ASSERT_EQ(1, object.anEvent.size());
}

TEST(EventTests, TestEquality)
{
    MyObj object;
    MyObj object2;
    ASSERT_TRUE(object == object);
    ASSERT_TRUE(object.anEvent == object2.anEvent);
    object.anEvent += func;
    object2.anEvent += func;
    ASSERT_TRUE(object.anEvent == object2.anEvent);
}
