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
    /// class instances and instance methods, Functors, or lambdas 
    /// that can be used as callbacks or event handlers.
    
    /// Here is a program that illustrates the use of the Delegate class:
    /// \include Delegate/Delegate.cpp
    /// @tparam result_t The result type that is returned from the delegates.
    /// @tparam arguments_t The argument types for any parameters for functions represented by the Delegate.
    template<typename result_t, typename... arguments_t>
    class Delegate
    {
        public:
            /// @brief function_t pointer type
            using function_t = std::function<result_t(arguments_t...)>;
            /// @brief Initializes an empty Delegate
            Delegate() = default;
            /// @brief Copy constructor
            /// @param other The Delegate object to copy.
            Delegate(const Delegate& other) noexcept
            {
                m_data->functions = other.m_data->functions;
            }
            /// @brief Move constructor
            /// @param other The Delegate object to move.
            Delegate(Delegate&& other) noexcept
            {
                m_data->functions = other.m_data->functions;
                other.m_data->functions.clear();
            }
            /// @brief Constructs a Delegate object from a function, static class method, or a Functor.
            /// @param function The function, static class method, or Functor to place as the first function
            /// in the new Delegate object.
            Delegate(const function_t& function)
            {
                m_data->functions.push_back(function);
            }
            /// @brief Constructor that takes const method with no parameters
            /// @tparam object_t The type of the class containing the method to store as the delegate.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)() const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object)));
            }
            /// @brief Constructor that takes a const method with one parameter
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t) const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1));
            }
            /// @brief Constructor that takes a const method with two parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's first parameter type.
            /// @tparam arg2_t The method's second parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t) const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2));
            }
            /// @brief Constructor that takes a const method with three parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's first parameter type.
            /// @tparam arg2_t The method's second parameter type.
            /// @tparam arg3_t The method's third parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t) const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            }
            /// @brief Constructor that takes a const method with four parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's first parameter type.
            /// @tparam arg2_t The method's second parameter type.
            /// @tparam arg3_t The method's third parameter type.
            /// @tparam arg4_t The method's fourth parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t,
                typename arg4_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t,
                arg4_t) const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                    std::placeholders::_4));
            }
            /// @brief Constructor that takes a const method with five parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's first parameter type.
            /// @tparam arg2_t The method's second parameter type.
            /// @tparam arg3_t The method's third parameter type.
            /// @tparam arg4_t The method's fourth parameter type.
            /// @tparam arg5_t The method's fifth parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t,
                typename arg4_t, typename arg5_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t,
                arg4_t, arg5_t) const) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                    std::placeholders::_4, std::placeholders::_5));
            }
            /// @brief Constructor that takes a non-const method with no parameters
            /// @tparam object_t The type of the class containing the method to store as the delegate.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t>
            Delegate(const object_t& object, result_t(object_t::*method)()) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object)));
            }
            /// @brief Constructor that takes a non-const method with one parameter
            /// @tparam object_t The type of the class containing the method to store as a delegate
            /// @tparam arg_t The method's parameter type
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t)) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1));
            }
            /// @brief Constructor that takes a non-const method with two parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The method's first parameter type.
            /// @tparam arg2_t The method's second parameter type.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t)) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2));
            }
            /// @brief Constructor that takes a non-const method with three parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The type of the method's first parameter.
            /// @tparam arg2_t The type of the method's second parameter.
            /// @tparam arg3_t The type of the method's third parameter.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t)) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            }
            /// @brief Constructor that takes a non-const method with four parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The type of the method's first parameter.
            /// @tparam arg2_t The type of the method's second parameter.
            /// @tparam arg3_t The type of the method's third parameter.
            /// @tparam arg4_t The type of the method's fourth parameter
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t,
                typename arg4_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t, 
                arg4_t)) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                    std::placeholders::_4));
            }
            /// @brief Constructor that takes a non-const method with five parameters
            /// @tparam object_t The type of the class containing the method to store as a delegate.
            /// @tparam arg1_t The type of the method's first parameter.
            /// @tparam arg2_t The type of the method's second parameter.
            /// @tparam arg3_t The type of the method's third parameter.
            /// @tparam arg4_t The type of the method's fourth parameter.
            /// @tparam arg5_t The type of the method's fifth parameter.
            /// @param object The class instance for the method.
            /// @param method The method to call.
            template<typename object_t, typename arg1_t, typename arg2_t, typename arg3_t,
                typename arg4_t, typename arg5_t>
            requires std::is_class_v<object_t>
            Delegate(const object_t& object, result_t(object_t::*method)(arg1_t, arg2_t, arg3_t, 
                arg4_t, arg5_t)) noexcept
            {
                m_data->functions.push_back(std::bind(method, const_cast<object_t*>(&object),
                    std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                    std::placeholders::_4, std::placeholders::_5));
            }
            /// @brief Destructor
            virtual ~Delegate() = default;
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
            /// @brief Remove all functions from the delegate
            void clear()
            {
                m_data->functions.clear();
            }
            /// @brief Return if the delegate is empty.
            /// @return true if delegate is empty, false otherwise.
            bool empty() const noexcept
            {
                return m_data->functions.empty();
            }
            /// @brief Invoke the methods represented by the current delegate.
            /// @param ...args The parameters to pass to each method.
            /// @return The return value from the last method call.
            result_t invoke(arguments_t... args) const
            {
                return operator ()(args...);
            }
            /// @brief Remove all functions in the input Delegate object from the functions 
            // in this object.
            /// @param other the Delegate object containing the functions to remove.
            /// @return the Delegate object containing the functions that are in this, but
            /// not in other.
            Delegate& remove(const Delegate& other) noexcept
            {
                for (const function_t& function : other.m_data->functions)
                {
                    remove(function);
                }
                return *this;
            }
            /// @brief Remove the function from this object
            /// @param function The function to remove.
            /// @return Delegate object containing functions that are in this instance 
            /// without function.
            Delegate& remove(const function_t& function)
            {
                std::erase_if(m_data->functions,
                    [&function](function_t& func) -> bool { return are_equal(func, function); });
                return *this;
            }
            /// @brief Compare two Delegates for equality
            /// @param other The second Delegate object to compare to this
            /// @return true if other contains the same delegates in the same order,
            /// false otherwise.
            bool operator ==(const Delegate& other)
            {
                if (size() != const_cast<Delegate&>(other).size())
                {
                    return false;
                }
                for (size_t index = 0; index < m_data->functions.size(); ++index)
                {
                    if (!are_equal(m_data->functions[index], other.m_data->functions[index]))
                    {
                        return false;
                    }
                }
                return true;
            }
            /// @brief Compares two Delegates for inequality
            /// @param other The second Delegate object to compare to this
            /// @return Return true if the Delegate objects do not contain the same delegates
            /// in the same order, otherwise false.
            bool operator !=(const Delegate& other)
            {
                return !operator ==(other);
            }
            /// @brief Retrieve the number of functions in the Delegate object
            /// @return The number of functions
            size_t size() { return m_data->functions.size(); }
            /// @brief Add the function specified by the parameter to this object.
            /// @param function The function to add.
            /// @return The Delegate object (this) that contains the functions that were in
            /// the original Delegate plus the function specified by the parameter.
            Delegate& operator +=(const function_t& function)
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
            /// @brief Remove the function specified by the parameter from this object.
            /// @param function The function to remove.
            /// @return The Delegate object (this) that contains the functions that were
            /// in the original Delegate object minus the function specified by the parameter.
            Delegate& operator -=(const function_t& function)
            {
                remove(function);
                return *this;
            }
            /// @brief Remove the functions in one Delegate object from this object
            /// @param delegate The Delegate object containing the funections to remove.
            /// @return The Delegate object (this) that contains the functions that were
            /// in the original Delegate minus the functions that are in the object
            /// specified by the parameter
            Delegate& operator -=(const Delegate& delegate)
            {
                remove(delegate);
                return *this;
            }
            /// @brief Invokes the functions in the current Delegate object.
            /// @return The value returned from executing the last function in the Delegate object.
            result_t operator ()(arguments_t... args) const
            {
                if (m_data->functions.empty())
                {
                    return result_t();
                }
                for (size_t index = 0; index < m_data->functions.size() - 1; ++index)
                {
                    m_data->functions[index](args...);
                }
                if (std::is_void_v<result_t>)
                {
                    m_data->functions[m_data->functions.size() -1](args...);
                }
                else
                {
                    return m_data->functions[m_data->functions.size() -1](args...);
                }
            }
        private:
            static bool are_equal(const function_t& left, const function_t& right)
            {
                return left.target_type() == right.target_type() && 
                    (left.template target<result_t(*)(arguments_t...)>() == right.template target<result_t(*)(arguments_t...)>()
                    || *left.template target<result_t(*)(arguments_t...)>() == *right.template target<result_t(*)(arguments_t...)>());
            }
            void combine(const Delegate& other)
            {
                std::ranges::copy(other.m_data->functions, std::back_inserter(m_data->functions));
            }
            void combine(const function_t& function)
            {
                m_data->functions.push_back(function);
            }
            struct data
            {
                std::vector<function_t> functions;
            };
            std::shared_ptr<data> m_data = std::make_shared<data>();
    };
}