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
    /// @remark This class serves as the base class for all classes that represent event
    /// data. For example, the jimo::timing::TickEventArgs class derives from EventArgs and
    /// is used to hold information whenever a Timer object tick event fires.
    /// @remark To create a
    /// custom event data class, create a class that derives from EventArgs and provides
    /// the properties to store the necessary data. The name of the custom class should end
    /// with <code>EventArgs</code>.
    /// @remark To pass an object that does not contain any data, pass an EventArgs object.
    /// @remark The Event.cpp example program uses both a derived NumberEventArgs class
    /// object and an empty EventArgs object.
    /// @example Event/Event1/Event1.cpp
    class EventArgs
    {
        public:
            /// @brief Initialize new instance of the EventArgs class.
            /// @remark Use an object of this type when you want to pass an object that
            /// contains no data.
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
            bool halt() const noexcept { return m_halt; }
            void setHalt() noexcept { m_halt = true; }
        private:
            bool m_halt{ false };
    };
}