/// @file Delegate.h
/// @author Jim Orcheson
/// @copyright 2022 Jim Orcheson. Use dictated by MIT License.
#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>
#include "EventArgs.h"

namespace jimo
{
    /// @brief Represents a delegate object, which is a data structure that refers to
    /// functions, static class methods, 
    /// class instances and instance methods, or Functors 
    /// that can be used as callbacks or event handlers.
    /// @tparam result_t The result type that is returned from the delegates.
    template<typename result_t>
    class Delegate
    {
        /// @brief delegate_t pointer type
        using delegate_t = std::function<result_t()>;
        public:
            /// @brief Initializes an empty Delegate
            Delegate() = default;
            /// @brief Copy constructor
            /// @param other The Delegate object to copy.
            Delegate(const Delegate& other)
            {
                m_data->functions = other.m_data->functions;
            }
            /// @brief Move constructor
            /// @param other The Delegate object to move.
            Delegate(Delegate&& other)
            {
                m_data->functions = other.m_data->functions;
                other.m_data->functions.clear();
            }
            /// @brief Constructs a Delegate object from a function, static class method, class instance and
            /// instance method, or a Functor.
            /// @param function 
            Delegate(const delegate_t& function)
            {
                m_data->functions.push_back(function);
            }
            /// @brief Copy equals operator
            /// @param other The Delegate object to copy
            /// @return Delegate object that contains a copy of the Delegates in the copied object.
            Delegate& operator =(const Delegate& other)
            {
                m_data->functions = other.m_data->functions;
                return *this;
            }
            /// @brief Move equals operator
            /// @param other The Delegate object to move
            /// @return Delegate object that contains the Delegates in the moved object.
            Delegate& operator =(Delegate&& other)
            {
                m_data->functions = other.m_data->functions;
                other.m_data->functions.clear();
                return *this;
            }
            /// @brief Retrieve the number of functions in the Delegate object
            /// @return The number of functions
            size_t size() { return m_data->functions.size(); }
            /// @brief Add the function specified by the parameter to this object.
            /// @param function The function to add.
            /// @return The Delegate object (this) that contains the functions that were in
            /// the original Delegate plus the function specified by the parameter.
            Delegate operator +=(delegate_t function)
            {
                combine(function);
                return *this;
            }
            /// @brief Add the functions in one Delegate object to this object
            /// @param delegate The Delegate object whose functions are to be added to this object
            /// @return The Delegate object (this) that contains the functions that were originally in
            /// the original Delegate plus the functions in the Delegate object that are being added.
            Delegate& operator +=(const Delegate& delegate)
            {
                combine(delegate);
                return *this;
            }
            /// @brief Invokes the functions in the current Delegate object.
            /// @return The value returned from executing the last function in the Delegate object.
            result_t operator ()() const
            {
                if (m_data->functions.empty() && !std::is_void_v<result_t>)
                {
                    return result_t();
                }
                for (size_t index = 0; index < m_data->functions.size() - 1; ++index)
                {
                    m_data->functions[index]();
                }
                if (std::is_void_v<result_t>)
                {
                    m_data->functions[m_data->functions.size() -1]();
                }
                else
                {
                    return m_data->functions[m_data->functions.size() -1]();
                }
            }
        private:
            void combine(const Delegate& other)
            {
                std::ranges::copy(other.m_data->functions, std::back_inserter(m_data->functions));
            }
            void combine(const delegate_t& function)
            {
                m_data->functions.push_back(function);
            }

            struct data
            {
                std::vector<delegate_t> functions;
            };
            std::shared_ptr<data> m_data = std::make_shared<data>();
    };
}