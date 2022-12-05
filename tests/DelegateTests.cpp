#include <gtest/gtest.h>
#include <iostream>
#include "Delegate.h"
#include "EventArgs.h"

using namespace jimo;

int func() { return 1; };
int func2(int x) { return x + 2; }

TEST(DelegateTests, TestCopyConstructor)
{
    Delegate<int> delegate;
    Delegate<int> delegate2(func);
    ASSERT_EQ(0, delegate.size());
    ASSERT_EQ(1, delegate2.size());
    Delegate<int> delegate3(delegate2);
    ASSERT_EQ(1, delegate3.size());
    ASSERT_EQ(1, delegate2.size());
}

TEST(DelegateTests, TestMoveConstructor)
{
    Delegate<int> delegate{ func };
    Delegate<int> delegate2(std::move(delegate));
    ASSERT_EQ(1, delegate2.size());
    ASSERT_EQ(0, delegate.size());
}

TEST(DelegateTests, TestCopyEquals)
{
    Delegate<int> delegate{ func };
    Delegate<int> delegate2 = delegate;
    ASSERT_EQ(1, delegate.size());
    ASSERT_EQ(1, delegate2.size());
}

TEST(DelegateTests, TestMoveEquals)
{
    Delegate<int> delegate{ func };
    Delegate<int> delegate2 = std::move(delegate);
    ASSERT_EQ(0, delegate.size());
    ASSERT_EQ(1, delegate2.size());
}

TEST(DelegateTests, TestDelegatePlusEquals)
{
    Delegate<int> delegate{ func };
    Delegate<int, int> delegate3( func2 );
    Delegate<int> delegate2;
    delegate2 += delegate;
    ASSERT_EQ(1, delegate2.size());
}

TEST(DelegateTests, TestFunctionPlusEquals)
{
    Delegate<int, int> delegate;
    delegate += func2;
    delegate += [](int x)->int{ return x + 4; };
    int result = delegate(1);
    ASSERT_EQ(2, delegate.size());
    ASSERT_EQ(5, result);
}

TEST(DelegateTests, TestExecute)
{
    struct Data
    {
        int one;
        int two;
    };

    Data data;
    Delegate<void> delegate([&data](){ data.one = 42; });
    delegate += [&data](){ data.two = 14; };
    delegate();
    ASSERT_EQ(42, data.one);
    ASSERT_EQ(14, data.two);

    Delegate<int> delegate2{ func };
    ASSERT_EQ(1, delegate2());
}

TEST(DelegateTests, TestMethodDelegate)
{
    class Object
    {
        public:
            int value() const noexcept
            {
                return m_value;
            }
            int value2() noexcept
            {
                return m_value2;
            }
            int addTwo(int x) const noexcept { return x + 2; }
            int anotherAddTwo(int x) noexcept { return x + 2; }
        private:
            int m_value = 42;
            int m_value2 = 14;
    };
    Object object;

    Delegate<int> delegate(object, &Object::value);
    Delegate<int> delegate2(object, &Object::value2);
    Delegate<int, int> delegate3(object, &Object::addTwo);
    Delegate<int, int> delegate4(object, &Object::anotherAddTwo);
    int value = delegate();
    int value2 = delegate2();
    int xPlusTwo = delegate3(5);
    int anotherXPlusTwo = delegate4(16);
    ASSERT_EQ(42, value);
    ASSERT_EQ(14, value2);
    ASSERT_EQ(7, xPlusTwo);
    ASSERT_EQ(18, anotherXPlusTwo);
}
