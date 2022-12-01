/// \file TimerException.h
#pragma once
#include <stdexcept>

namespace jimo::timing
{
    /// \class TimerException
    /// \brief Exception class for all Timer error conditions
    ///
    /// Objects of this type contain an exception message only. It would have been
    /// nice to include a stack trace, but there is no standard way of obtaining a stack
    /// trace
    /// in C++20. A standard stacktrace class will be added to C++23, so it may be added
    /// to this class when multiple compiler libraries have implemented it.
    /// \note TimerException is derived from std::runtime_error.
    /// See the <A HREF="https://en.cppreference.com/w/cpp/error/runtime_error">documentation</A>
    /// for that class for more functionality.
    class TimerException : public std::runtime_error
    {
        public:
            /// \name Constructors
           ///@{
            ///
            /// \param what_arg String containing the error message.
            TimerException(const std::string& what_arg)
                : std::runtime_error(what_arg) {}
            /// \param what_arg Array of characters that contain the error message.
            TimerException(const char* what_arg)
                : std::runtime_error(what_arg) {}
            ///@}
    };
}