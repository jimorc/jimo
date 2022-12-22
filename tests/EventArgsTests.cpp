/// @file EventArgsTests.cpp
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#include "EventArgs.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace jimo;

class TestEventArgs : public EventArgs
{
    public:
        TestEventArgs(int data) : EventArgs(), m_testData(data) {}
        virtual ~TestEventArgs() noexcept {}
        int TestData() const noexcept { return m_testData; }
        virtual bool operator==(const EventArgs& other) const noexcept override
        { 
            try
            {
                const auto& args = dynamic_cast<const TestEventArgs&>(other);
                return (m_testData == args.m_testData);
            }
            catch (const std::bad_cast&)
            {
                return false;
            }
        }
        virtual std::string ToString() const override
        {
            std::stringstream ss;
            ss << "TestEventArgs: " << m_testData;
            return ss.str();
        }
    private:
        int m_testData;
};

TEST(EventArgsTests, TestEquals)
{
    EventArgs eventArgs;
    EventArgs eventArgs2;
    TestEventArgs testEventArgs(10);
    TestEventArgs testEventArgs2(6);
    TestEventArgs testEventArgs3(10);

    ASSERT_TRUE(eventArgs == eventArgs2);
    ASSERT_FALSE(testEventArgs == eventArgs);
    ASSERT_FALSE(eventArgs == testEventArgs);
    ASSERT_TRUE(testEventArgs == testEventArgs3);
    ASSERT_FALSE(testEventArgs == testEventArgs2);
}

TEST(EventArgsTests, TestNotEquals)
{
    EventArgs eventArgs;
    EventArgs eventArgs2;
    TestEventArgs testEventArgs(10);
    TestEventArgs testEventArgs2(6);
    TestEventArgs testEventArgs3(10);

    ASSERT_FALSE(eventArgs != eventArgs2);
    ASSERT_TRUE(testEventArgs != eventArgs);
    ASSERT_TRUE(eventArgs != testEventArgs);
    ASSERT_FALSE(testEventArgs != testEventArgs3);
    ASSERT_TRUE(testEventArgs != testEventArgs2);
}

TEST(EventArgsTests, TestToString)
{
    EventArgs eventArgs;
    TestEventArgs testEventArgs(5);

    ASSERT_STREQ("jimo::EventArgs", eventArgs.ToString().c_str());
    ASSERT_STREQ("TestEventArgs: 5", testEventArgs.ToString().c_str());
}