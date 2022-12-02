/// @file EventArgs.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#pragma once
#include <concepts>
#include <string>

namespace jimo
{
    /// @class EventArgs
    /// @brief Represents the base class for classes that contain event data.
    ///
    /// Also provides a value to use for events that do not include event data.
    class EventArgs
    {
        public:
            /// @brief Default constructor
            /// @remark This constructor should be called from all derived EventArgs classes.
            /// @remark It is also called directly in the empty static EventArgs object.
            EventArgs() = default;
            /// @brief Copy constructor
            /// @param other the EventArgs object to copy
            EventArgs(const EventArgs& other) = default;
            /// @brief Move constructor
            /// @param other the EventArgs object to move
            EventArgs(EventArgs&& other) = default;
            /// @brief Destructor
            virtual ~EventArgs() noexcept {}
            /// @brief Copy equals operator
            /// @param other the EventArgs object to copy
            /// @return the copied EventArgs object
            inline EventArgs& operator =(const EventArgs& other) = default;
            /// @brief Move equals operator
            /// @param other the EventArgs object to move
            /// @return the moved EventArgs object
            inline EventArgs& operator =(EventArgs&& other) = default;

            /// @brief Determine if the contents of the eventArgs objects are the same.
            /// @param other The EventArgs object to compare for equality.
            /// @return true if both objects are base EventArgs objects. This is equivalent to
            /// testing if the two objects contain the same data because base EventArgs objects
            /// have no data fields.
            /// @remark This operator should be overridden for all derived types to check for matching
            /// field values.
            virtual bool operator==(const EventArgs& other) const noexcept
            { 
                return std::is_same_v<decltype(*this), decltype(other)>;
            }
            /// @brief Determine if the contents of the EventArgs objects are different
            /// @param other The EventArgs object to compare for inequality.
            /// @return true if the objects are not both the same EventArgs types.
            virtual bool operator!=(const EventArgs& other) const noexcept
            {
                return !operator==(other);
            }
            /// @brief Returns a string representing the current EventArgs object.
            /// @return A string that represents the current object.
            virtual std::string ToString() const
            {
                return "jimo::EventArgs";
            }
            /// @brief Provides a value to use with events that do not have event data.
            /// @remark Pass this value to event handlers that are associated with events that
            /// do not have data.
            static const EventArgs empty;
    };
}