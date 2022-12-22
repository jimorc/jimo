/// @file Equatable.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.

#pragma once

namespace jimo
{
    /// @brief Abstract class that defines an equatable interface.
    ///
    /// The class contains one pure virtual method called <code>equals</code> that must
    /// be defined in derived classes to compare one Equatable object with another.
    /// @tparam type_t Class type of the two objects that are being compared. The two
    /// objects must be of the same class.
    template<typename type_t> 
    class Equatable
    {   
        public:
            /// @brief Destructor
            virtual ~Equatable() = default;
            /// @brief Check if two objects are equal. 
            /// @param other The object to compare this to.
            /// @return <code>true</code> if objects are equal, <code>false</code> otherwise.
            virtual auto equals(const type_t& other) const noexcept -> bool = 0;
            /// @brief Equals operator
            /// 
            /// Compares two objects for equality.
            /// @param left First object to compare.
            /// @param right Second object to compare to the first.
            /// @return <code>true</code> if objects are equal, <code>false</code> otherwise.
            friend auto operator ==(const type_t& left, const type_t& right) noexcept -> bool
            { 
                return left.equals(right); 
            }
            /// @brief Check if two objects are not equal
            /// @param left First object to compare.
            /// @param right Second object to compare to the first.
            /// @return <code>true</code> if objects are not equal, <code>false</code> if they are equal.
            friend auto operator !=(const type_t& left, const type_t& right) noexcept -> bool
            {
                return !left.equals(right); 
            }
    };
}