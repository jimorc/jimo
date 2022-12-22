/// @file Object.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#pragma once
#include "Equatable.h"

namespace jimo
{
    /// @brief Base Object class.
    ///
    /// All objects that call event handlers must be derived from this class.
    class Object : public Equatable<Object>
    {
        public:
            /// @brief Constructor
            Object() = default;
            /// @brief Destructor
            virtual ~Object() = default;
            /// @brief Copy equals operator
            /// @param other The Object to copy
            /// @return This object.
            virtual auto operator =(const Object& other) -> Object& = default;
            /// @brief Compares an Object with this object
            /// @param other The Object to compare to this.
            /// @return <code>true</code> if the objects are equal, <code>false</code> otherwise
            virtual auto equals(const Object& other) const noexcept -> bool
            {
                return this == &other;
            }
    };
}
