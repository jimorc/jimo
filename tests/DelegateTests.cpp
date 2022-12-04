#include <gtest/gtest.h>
#include <iostream>
#include "Delegate.h"
#include "EventArgs.h"

using namespace jimo;

using delegate_t = std::function<int()>;

delegate_t func = []() { return 1; };

TEST(DelegateTests, TestCopyConstructor)
{
    Delegate<int> delegate;
    Delegate<int> delegate2{ func };
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
    Delegate<int> delegate2;
    delegate2 += delegate;
    ASSERT_EQ(1, delegate2.size());
}

TEST(DelegateTests, TestFunctionPlusEquals)
{
    Delegate<int> delegate;
    delegate += func;
    delegate += delegate_t([](){ return 2; });
    ASSERT_EQ(2, delegate.size());
}

TEST(DelegateTests, TestExecute)
{
    using dataDelegate_t = std::function<void()>;
    struct Data
    {
        int one;
        int two;
    };

    Data data;
    Delegate<void> delegate(dataDelegate_t([&data](){ data.one = 42; }));
    delegate += dataDelegate_t([&data](){ data.two = 14; });
    delegate();
    ASSERT_EQ(42, data.one);
    ASSERT_EQ(14, data.two);

    Delegate<int> delegate2{ func };
    ASSERT_EQ(1, delegate2());
}