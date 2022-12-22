/// @file ObjectTests.cpp
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#include <gtest/gtest.h>
#include "Object.h"

using namespace jimo;

TEST(ObjectTests, TestEquals)
{
    Object object;
    Object object2;
    class Derived : public Object
    {
        public:
            bool operator ==(const Object& other)
            {
                return Object::equals(other);
            }
    };
    Derived derived;
    ASSERT_TRUE(object == object);
    ASSERT_FALSE(object == object2);
    ASSERT_FALSE(derived == object);
    ASSERT_FALSE(object2 == derived);
}

TEST(ObjectTests, TestNotEquals)
{
    Object object;
    Object object2;
    class Derived : public Object
    {
        public:
            bool operator ==(const Object& other)
            {
                return Object::equals(other);
            }
    };
    Derived derived;
    ASSERT_FALSE(object != object);
    ASSERT_TRUE(object != object2);
    ASSERT_TRUE(derived != object);
    ASSERT_TRUE(object2 != derived);
}