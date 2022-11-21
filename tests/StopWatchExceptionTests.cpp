#include "StopWatchException.h"
#include <gtest/gtest.h>

using namespace jimo::timing;

TEST(StopWatchExceptionTests, TestStopWatchExceptionString)
{
    EXPECT_THROW({
        try
        {
            std::string exceptionMessage("I threw an exception");
            throw StopWatchException(exceptionMessage);

        }
        catch (const StopWatchException& e)
        {
            ASSERT_STREQ(e.what(), "I threw an exception");
            throw;
        }
    }, StopWatchException);
}

TEST(StopWatchExceptionTests, TestStopWatchExceptionCharArray)
{
    EXPECT_THROW({
        try
        {
            throw StopWatchException("I threw an exception");

        }
        catch (const StopWatchException& e)
        {
            ASSERT_STREQ(e.what(), "I threw an exception");
            throw;
        }
    }, StopWatchException);

}