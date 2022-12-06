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
    class Object
    {
        public:
            static int addTwo(int x) noexcept { return x + 2; }
    };
    Delegate<int> delegate{ func };
    Delegate<int, int> delegate3( func2 );
    Delegate<int> delegate2;
    delegate2 += delegate;
    delegate3 += &Object::addTwo;
    ASSERT_EQ(1, delegate2.size());
    ASSERT_EQ(2, delegate3.size());
    ASSERT_EQ(6, delegate3(4));
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
        int three;
        void setThree(int x)
        {
            three = x;
        }
    };

    Data data;
    Delegate<void, int> delegate([&data](int) noexcept { data.one = 42; });
    delegate += [&data](int) noexcept { data.two = 14; };
    delegate += Delegate<void, int>(data, &Data::setThree);
    delegate(4);
    ASSERT_EQ(42, data.one);
    ASSERT_EQ(14, data.two);
    ASSERT_EQ(4, data.three);

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
            int addTwoNumbers(int x, int y) const noexcept { return x + y; }
            int anotherAddTwoNumbers(int x, int y) noexcept { return x + y; }
            float addFloatAndInts(float x, int y, int z) const noexcept { return x + y + z; }
            float anotherAddFloatAndInts(float x, int y, int z) noexcept { return x + y + z; }
            float addFourFloats(float a, float b, float c, float d) const noexcept { return a + b + c + d; }
            float add4Floats(float a, float b, float c, float d) noexcept { return a + b + c + d; }
            int addFiveInts(int a, int b, int c, int d, int e) const noexcept { return a + b + c + d + e; }
            int add5Ints(int a, int b, int c, int d, int e) noexcept { return a + b + c + d + e; }

        private:
            int m_value = 42;
            int m_value2 = 14; 
    };
    Object object;

    Delegate<int> delegate(object, &Object::value);
    Delegate<int> delegate2(object, &Object::value2);
    Delegate<int, int> delegate3(object, &Object::addTwo);
    Delegate<int, int> delegate4(object, &Object::anotherAddTwo);
    Delegate<int, int, int> delegate5(object, &Object::addTwoNumbers);
    Delegate<int, int, int> delegate6(object, &Object::anotherAddTwoNumbers);
    Delegate<float, float, int, int> delegate7(object, &Object::addFloatAndInts);
    Delegate<float, float, int, int> delegate8(object, &Object::anotherAddFloatAndInts);
    Delegate<float, float, float, float, float> delegate9(object, &Object::addFourFloats);
    Delegate<float, float, float, float, float> delegate10(object, &Object::add4Floats);
    Delegate<int, int, int, int, int, int> delegate11(object, &Object::addFiveInts);
    Delegate<int, int, int, int, int, int> delegate12(object, &Object::add5Ints);
    ASSERT_EQ(42, delegate());
    ASSERT_EQ(14, delegate2());
    ASSERT_EQ(7, delegate3(5));
    ASSERT_EQ(18, delegate4(16));
    ASSERT_EQ(8, delegate5(5, 3));
    ASSERT_EQ(25, delegate6(12, 13));
    ASSERT_EQ(32.f, delegate7(25.f, 4, 3));
    ASSERT_EQ(19.f, delegate8(9.f, 6, 4));
    ASSERT_EQ(7.0f, delegate9(3.0f, 2.0f, 5.0f, -3.0f));
    ASSERT_EQ(11.0f, delegate10(4.0f, 8.0f, 5.0f, -6.0f));
    ASSERT_EQ(2, delegate11(3, 2, 5, -14, 6));
    ASSERT_EQ(3, delegate12(6, 3, -10, -3, 7));
}

TEST(DelegateTests, TestFunctorsAdd)
{
    class AddThreeFunctor
    {
        public:
            int operator ()(int x)
            {
                return x + 3;
            }
    };
    AddThreeFunctor addThree;
    Delegate<int, int> delegate(addThree);
    delegate += addThree;

    ASSERT_EQ(7, delegate(4));
}

TEST(DelegateTests, TestClear)
{
    Delegate<void> delegate([](){ /* do something */ });
    delegate += [](){ /* do something else */ };
    EXPECT_EQ(2, delegate.size());

    delegate.clear();
    ASSERT_EQ(0, delegate.size());
}

TEST(DelegateTests, TestInvoke)
{
    Delegate<int, int>delegate([](int x){ return x + 4; });
    delegate += [](int x){ return x * x; };
    ASSERT_EQ(9, delegate(3));
}

TEST(DelegateTests, TestEmpty)
{
    Delegate<int, int> delegate;
    ASSERT_TRUE(delegate.empty());
    delegate += [](int x) -> int { return x; };
    ASSERT_FALSE(delegate.empty());
}
