// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// NOLINTBEGIN(modernize-avoid-c-arrays)

#ifndef BASE_PYARGUMENTS_H
#define BASE_PYARGUMENTS_H

#include <array>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif

#ifndef PY_CXX_CONST
#define PY_CXX_CONST const
#endif

#include <Python.h>

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Base::Py Namespace                                                       │
// └──────────────────────────────────────────────────────────────────────────┘

namespace Base::Py
{

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║ Private implementation details                                           ║
// ╚══════════════════════════════════════════════════════════════════════════╝

namespace detail // Private implementation details
{

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Variadic type packs utils                                                │
// └──────────────────────────────────────────────────────────────────────────┘

// List of types: type_list<int, float, const char*, ...> {}
template <typename... Ts>
struct type_list
{};

// Concatenate two lists of types (Deduction guide)
template <typename... Ts, typename... Us>
constexpr auto concat_types(type_list<Ts...>, type_list<Us...>) -> type_list<Ts..., Us...>;

// flatmap of list of parse types: The recursive case
// Concatenate all parse types from arguments into a single list of types
template <typename T, typename... REST>
struct expand_parse_types
{
    using head_type_list = typename T::parse_type;
    using tail_type_list = typename expand_parse_types<REST...>::parse_type;
    using parse_type
        = decltype(concat_types(std::declval<head_type_list>(), std::declval<tail_type_list>()));
};

// flatmap of list of parse types: The base case for the recursion
template <typename T>
struct expand_parse_types<T>
{
    using parse_type = typename T::parse_type;
};

// flatmap of list of value types: The recursive case
// Concatenate all value types from arguments into a single list of types
template <typename T, typename... REST>
struct expand_value_types
{
    using head_type_list = typename T::value_type;
    using tail_type_list = typename expand_value_types<REST...>::value_type;
    using value_type
        = decltype(concat_types(std::declval<head_type_list>(), std::declval<tail_type_list>()));
};

// flatmap of list of value types: The base case for the recursion
template <typename T>
struct expand_value_types<T>
{
    using value_type = typename T::value_type;
};

// flatmap of list of named Argument types: The recursive case
// Concatenate all named argument types into a single list of types.
template <typename T, typename... REST>
struct expand_arg_types
{
    using head_type_list = type_list<T>;
    using tail_type_list = typename expand_arg_types<REST...>::type;
    using type = std::conditional_t<T::named,
                                    decltype(concat_types(std::declval<head_type_list>(),
                                                          std::declval<tail_type_list>())),
                                    tail_type_list>;
};

// flatmap of list of named Argument types: The base case for the recursion
template <typename T>
struct expand_arg_types<T>
{
    using type = std::conditional_t<T::named, type_list<T>, type_list<>>;
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Tuple utils                                                              │
// └──────────────────────────────────────────────────────────────────────────┘

// Tuple type builder from type_list
template <typename T>
struct type_list_to_tuple;

// Tuple type builder from type_list
template <typename... Ts>
struct type_list_to_tuple<type_list<Ts...>>
{
    using type = std::tuple<Ts...>;
};

// Trait to check if type T has a member named 'default_value'
template <typename T, typename = void>
struct has_default_value : std::false_type
{};

// Check presence of default_value
template <typename T>
struct has_default_value<T, std::void_t<decltype(std::declval<T&>().default_value)>>
    : std::true_type
{};

// Trait to check if type T has a 'clean' static template method
template <typename T, typename = void>
struct has_clean_method : std::false_type
{};

// Check presence of clean method by attempting to call it with dummy args
template <typename T>
struct has_clean_method<
    T,
    std::void_t<decltype(T::template clean<0>(std::declval<std::tuple<int>&>()))>> : std::true_type
{};

// Trait to check if type T has a static name member
template <typename T, typename = void>
struct has_name_member : std::false_type
{};

template <typename T>
struct has_name_member<T, std::void_t<decltype(T::name)>> : std::true_type
{};

// Check custom parse pointer value
template <typename T, typename = void>
struct has_parse_ptr_value : std::false_type
{};

template <typename T>
struct has_parse_ptr_value<T, std::void_t<decltype(T::parse_ptr_value())>> : std::true_type
{};

template <typename T>
constexpr bool has_name_member_v = has_name_member<T>::value;

template <typename T>
constexpr bool has_clean_method_v = has_clean_method<T>::value;

template <typename T>
constexpr bool has_default_value_v = has_default_value<T>::value;

// Helper trait to check if Fn is callable with the types in Tuple
template <typename Fn, typename Tuple>
struct is_callable_with_tuple;

template <typename Fn, typename... Ts>
struct is_callable_with_tuple<Fn, std::tuple<Ts...>> : std::is_invocable<Fn, Ts...>
{};

// Convenience alias
template <typename Fn, typename Tuple>
constexpr bool is_callable_with_tuple_v = is_callable_with_tuple<Fn, Tuple>::value;

template <typename T>
struct arg_traits
{
    static constexpr bool has_name = has_name_member_v<T>;
    static constexpr bool has_clean = has_clean_method_v<T>;
    static constexpr bool has_default = has_default_value_v<T>;
};

// Call init on all parser values
template <std::size_t Index = 0, std::size_t Pos = 0, typename... Args, typename Parsed>
void apply_init_helper(Parsed& parsed, const std::tuple<Args...>* args = nullptr)
{
    if constexpr (Index < sizeof...(Args))
    {
        using arg_t = std::tuple_element_t<Index, std::tuple<Args...>>;
        using traits = arg_traits<arg_t>;

        if constexpr (traits::has_default)
        {
            arg_t::template init<Pos>(parsed, std::get<Index>(*args).default_value);
        }
        else
        {
            arg_t::template init<Pos>(parsed);
        }
        apply_init_helper<Index + 1, Pos + arg_t::offset>(parsed, args);
    }
}

template <typename... Args, typename Parsed>
void apply_inits(Parsed& parsed, const std::tuple<Args...>* args = nullptr)
{
    apply_init_helper(parsed, args);
}

// Call clean on all parser values if available
template <std::size_t Index = 0, std::size_t Pos = 0, typename... Args, typename Parsed>
void apply_clean_helper(Parsed& parsed, const std::tuple<Args...>* args = nullptr)
{
    if constexpr (Index < sizeof...(Args))
    {
        using arg_t = std::tuple_element_t<Index, std::tuple<Args...>>;
        using traits = arg_traits<arg_t>;

        if constexpr (traits::has_clean)
        {
            arg_t::template clean<Pos>(parsed);
        }
        apply_clean_helper<Index + 1, Pos + arg_t::offset>(parsed, args);
    }
}

template <typename... Args, typename Parsed>
void apply_clean(Parsed& parsed, const std::tuple<Args...>* args = nullptr)
{
    apply_clean_helper(parsed, args);
}

// Copy values from parsed to values calling its type::get
template <std::size_t Index = 0,
          std::size_t Pos = 0,
          typename... Args,
          typename Values,
          typename Parsed>
void apply_get_helper(Parsed& parsed, Values& values, const std::tuple<Args...>* args = nullptr)
{
    if constexpr (Index < sizeof...(Args))
    {
        using arg_t = std::tuple_element_t<Index, std::tuple<Args...>>;
        std::get<Index>(values) = arg_t::template get<Pos>(parsed);
        apply_get_helper<Index + 1, Pos + arg_t::offset>(parsed, values, args);
    }
}

template <typename... Args, typename Values, typename Parsed>
void apply_gets(Parsed& parsed, Values& values, const std::tuple<Args...>* args = nullptr)
{
    apply_get_helper(parsed, values, args);
}

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Argument utils                                                           │
// └──────────────────────────────────────────────────────────────────────────┘

// format string builder from a list of argument types
// Build the final format string from all arguments.
template <typename... Ts>
constexpr auto args_fmt = (Ts::fmt + ...);

// Count named arguments
template <typename... Ts>
constexpr auto count_keywords = (0 + ... + (std::decay_t<Ts>::named ? 1 : 0));

// Build the keywords array from named arguments
template <typename... Ts>
constexpr auto build_keywords(Ts&&... args)
{
    constexpr size_t size = count_keywords<Ts...> + 1; // +1 for nullptr
    std::array<const char*, size> result {};

    size_t index = 0;
    auto add_if_valid = [&](auto&& arg) {
        using A = std::decay_t<decltype(arg)>;
        if constexpr (A::named)
        {
            result[index++] = arg.name;
        }
    };

    (add_if_valid(args), ...);
    result[index] = nullptr; // Null-terminate
    return result;
}

// Builder for the named arguments (strip markers)
// Recursive helper to build tuple of named args
template <std::size_t I = 0, typename... Ts>
constexpr auto build_named_args_impl(const std::tuple<Ts...>& args)
{
    if constexpr (I == sizeof...(Ts))
    {
        return std::tuple<> {};
    }
    else
    {
        using Current = std::tuple_element_t<I, std::tuple<Ts...>>;
        auto current_arg = std::get<I>(args);
        if constexpr (Current::named)
        {
            // Include this arg and recurse
            return std::tuple_cat(std::make_tuple(current_arg), build_named_args_impl<I + 1>(args));
        }
        else
        {
            // Skip this arg and recurse
            return build_named_args_impl<I + 1>(args);
        }
    }
}

// Build a tuple of named arguments (similar to build_keywords but returns tuple of args)
template <typename... Ts>
constexpr auto build_named_args(Ts&&... args)
{
    auto args_tuple = std::make_tuple(std::forward<Ts>(args)...);
    return build_named_args_impl(args_tuple);
}

// Base type for markers
struct marker_arg
{
    using parse_type = type_list<>;
    using value_type = type_list<>;
    static constexpr bool named {false};
};

template <typename T>
constexpr auto parse_ptr(T&& obj)
{
    using Type = std::decay_t<T>;

    if constexpr (has_parse_ptr_value<Type>::value)
    {
        return Type::parse_ptr_value();
    }
    else
    {
        return &obj;
    }
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ PyArg_ParseTupleAndKeywords wrapper                                      │
// └──────────────────────────────────────────────────────────────────────────┘

template <typename Tuple, std::size_t... I>
auto PyArg_ParseTupleAndKeywords_Impl(PyObject* args,
                                      PyObject* kwArgs,
                                      const char* fmt,
                                      char** keywords,
                                      Tuple&& tup,
                                      std::index_sequence<I...> _seq) -> int
{
    return PyArg_ParseTupleAndKeywords(
        args, kwArgs, fmt, keywords, parse_ptr(std::get<I>(std::forward<Tuple>(tup)))...);
}

template <typename... Args, std::size_t N>
auto PyArg_ParseTupleAndKeywords_Tuple(PyObject* args,
                                       PyObject* kwArgs,
                                       const char* fmt,
                                       const std::array<const char*, N> keywords,
                                       const std::tuple<Args...>& tup) -> int
{
    return PyArg_ParseTupleAndKeywords_Impl(args,
                                            kwArgs,
                                            fmt,
                                            const_cast<char**>(keywords.data()),
                                            tup,
                                            std::index_sequence_for<Args...> {});
}

} // namespace detail

// ╔══════════════════════════════════════════════════════════════════════════╗
// ║ Public API                                                               ║
// ╚══════════════════════════════════════════════════════════════════════════╝

// c-string alias for readability
using c_str_t = const char*;

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Constexpr string utils                                                   │
// └──────────────────────────────────────────────────────────────────────────┘

// Compile time string
template <std::size_t N>
struct FmtString
{
    constexpr FmtString(const char (&str)[N])
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            value[i] = str[i];
        }
    }

    constexpr FmtString() = default;

    char value[N] {};
};

// Deduction guide to help the compiler
template <std::size_t N>
FmtString(const char (&)[N]) -> FmtString<N>;

// Concatenation over FmtString
template <std::size_t N1, std::size_t N2>
constexpr auto operator+(const FmtString<N1>& lhs, const FmtString<N2>& rhs)
{
    FmtString<N1 + N2 - 1> result {}; // -1 avoids double '\0'
    for (std::size_t i = 0; i < N1 - 1; ++i)
    {
        result.value[i] = lhs.value[i];
    }
    for (std::size_t i = 0; i < N2; ++i)
    {
        result.value[i + N1 - 1] = rhs.value[i];
    }
    return result;
}

// Base type of arguments with name (keyword)
struct named_arg
{
    const char* name {};
    static constexpr bool named {true};
};

// Base type of arguments with default value
template <typename T>
struct with_default
{
    T default_value {};
};

// Basic argument with a single parsed value
template <typename T>
struct value_arg
{
    static constexpr std::size_t offset = 1;

    using value_type = detail::type_list<T>;
    using parse_type = detail::type_list<T>;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = T {};
    }

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple, const T& default_value)
    {
        std::get<Offset>(tuple) = default_value;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> T
    {
        return std::get<Offset>(tuple);
    }
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Marker Arguments                                                         │
// └──────────────────────────────────────────────────────────────────────────┘

// Marker of end of positional-only arguments
struct PosOnly
{};

// Marker of start of keyword-only arguments
struct KwOnly
{};

// Marker of start of optional arguments
struct Optional
{};

// Marker of Python boolean value
struct Bool
{};

// Marker of Varargs tuple
struct Varargs
{};

// Marker of Keywords dict
struct Keywords
{};

// Python tuple
struct Tuple
{};

// Python Dict
struct Dict
{};

// Python Py_ssize_t
struct SSize
{};

// Non negative byte
struct NNByte
{};

// 1 byte  (bytes[1]) to char
struct Byte1
{};

// 1 char (str) to int
struct Char1
{};

// Filesystem Path
struct FSPath
{
    static constexpr auto parse_ptr_value() { return PyUnicode_FSConverter; }
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Argument specializations                                                 │
// └──────────────────────────────────────────────────────────────────────────┘

// Validated python type
template <typename T>
struct Arg : named_arg
{
    using value_type = detail::type_list<T*>;
    using parse_type = detail::type_list<PyTypeObject&, PyObject*>;

    static constexpr PyTypeObject& py_type = T::Type;
    static constexpr FmtString fmt {"O!"};
    static constexpr std::size_t offset = 2;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = py_type;
        std::get<Offset + 1>(tuple) = nullptr;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> T*
    {
        return static_cast<T*>(std::get<Offset + 1>(tuple));
    }
};

// Python tuple
template <>
struct Arg<Tuple> : named_arg
{
    using value_type = detail::type_list<PyTupleObject*>;
    using parse_type = detail::type_list<PyTypeObject&, PyTupleObject*>;

    static constexpr FmtString fmt {"O!"};
    static constexpr std::size_t offset = 2;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = PyTuple_Type;
        std::get<Offset + 1>(tuple) = nullptr;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> PyTupleObject*
    {
        return static_cast<PyTupleObject*>(std::get<Offset + 1>(tuple));
    }
};

// Python dict
template <>
struct Arg<Dict> : named_arg
{
    using value_type = detail::type_list<PyDictObject*>;
    using parse_type = detail::type_list<PyTypeObject&, PyDictObject*>;

    static constexpr FmtString fmt {"O!"};
    static constexpr std::size_t offset = 2;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = PyDict_Type;
        std::get<Offset + 1>(tuple) = nullptr;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> PyDictObject*
    {
        return static_cast<PyDictObject*>(std::get<Offset + 1>(tuple));
    }
};

// Position-only marker
template <>
struct Arg<PosOnly> : detail::marker_arg
{
    static constexpr FmtString fmt {""};
};

// Keyword-only marker
template <>
struct Arg<KwOnly> : detail::marker_arg
{
    static constexpr FmtString fmt {"$"};
};

// Optional marker
template <>
struct Arg<Optional> : detail::marker_arg
{
    static constexpr FmtString fmt {"|"};
};

// byte argument
template <>
struct Arg<NNByte> : named_arg, with_default<unsigned char>, value_arg<unsigned char>
{
    static constexpr FmtString fmt {"b"};
};

template <>
struct Arg<unsigned char> : named_arg, with_default<unsigned char>, value_arg<unsigned char>
{
    static constexpr FmtString fmt {"B"};
};

// short argument
template <>
struct Arg<short> : named_arg, with_default<short>, value_arg<short>
{
    static constexpr FmtString fmt {"h"};
};

template <>
struct Arg<unsigned short> : named_arg, with_default<unsigned short>, value_arg<unsigned short>
{
    static constexpr FmtString fmt {"H"};
};

// int argument
template <>
struct Arg<int> : named_arg, with_default<int>, value_arg<int>
{
    static constexpr FmtString fmt {"i"};
};

template <>
struct Arg<unsigned int> : named_arg, with_default<unsigned int>, value_arg<unsigned int>
{
    static constexpr FmtString fmt {"I"};
};

// long argument
template <>
struct Arg<long> : named_arg, with_default<long>, value_arg<long>
{
    static constexpr FmtString fmt {"l"};
};

template <>
struct Arg<unsigned long> : named_arg, with_default<unsigned long>, value_arg<unsigned long>
{
    static constexpr FmtString fmt {"k"};
};

// long long argument
template <>
struct Arg<long long> : named_arg, with_default<long long>, value_arg<long long>
{
    static constexpr FmtString fmt {"L"};
};

template <>
struct Arg<unsigned long long>
    : named_arg, with_default<unsigned long long>, value_arg<unsigned long long>
{
    static constexpr FmtString fmt {"K"};
};

// Py_ssize_t argument
template <>
struct Arg<SSize> : named_arg, with_default<Py_ssize_t>, value_arg<Py_ssize_t>
{
    static constexpr FmtString fmt {"n"};
};

// char from bytes[1]
template <>
struct Arg<Byte1> : named_arg, with_default<char>, value_arg<char>
{
    static constexpr FmtString fmt {"c"};
};

// int from str[1]
template <>
struct Arg<Char1> : named_arg, with_default<int>, value_arg<int>
{
    static constexpr FmtString fmt {"C"};
};

// float argument
template <>
struct Arg<float> : named_arg, with_default<float>, value_arg<float>
{
    static constexpr FmtString fmt {"f"};
};

// double argument
template <>
struct Arg<double> : named_arg, with_default<double>, value_arg<double>
{
    static constexpr FmtString fmt {"d"};
};

// Generic python object argument
template <>
struct Arg<PyObject*> : named_arg, value_arg<PyObject*>
{
    static constexpr FmtString fmt {"O"};
};

// boolean argument
template <>
struct Arg<Bool> : named_arg, with_default<int>, value_arg<int>
{
    static constexpr FmtString fmt {"p"};
};

// string_view argument
template <>
struct Arg<std::string_view> : named_arg
{
    static constexpr FmtString fmt {"s#"};
    static constexpr std::size_t offset = 2;

    using value_type = detail::type_list<std::string_view>;
    using parse_type = detail::type_list<c_str_t, Py_ssize_t>;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = nullptr;
        std::get<Offset + 1>(tuple) = 0;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> std::string_view
    {
        std::string_view sview {std::get<Offset>(tuple),
                                static_cast<unsigned long>(std::get<Offset + 1>(tuple))};
        return sview;
    }
};

// string argument
template <>
struct Arg<std::string> : named_arg
{
    static constexpr FmtString fmt {"s#"};
    static constexpr std::size_t offset = 2;

    using value_type = detail::type_list<std::string>;
    using parse_type = detail::type_list<c_str_t, Py_ssize_t>;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = nullptr;
        std::get<Offset + 1>(tuple) = 0;
    }

    template <std::size_t Offset, typename... Args>
    static auto get(std::tuple<Args...>& tuple) -> std::string
    {
        std::string str {std::get<Offset>(tuple),
                         static_cast<unsigned long>(std::get<Offset + 1>(tuple))};
        return str;
    }
};

// c-string argument (null terminated)
template <>
struct Arg<c_str_t> : named_arg, with_default<c_str_t>, value_arg<c_str_t>
{
    static constexpr FmtString fmt {"s"};
};

// filesystem path argument
template <>
struct Arg<FSPath> : named_arg
{
    static constexpr FmtString fmt {"O&"};
    static constexpr std::size_t offset = 2;

    using value_type = detail::type_list<std::string_view>;
    using parse_type = detail::type_list<FSPath, PyObject*>;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = FSPath {};
        std::get<Offset + 1>(tuple) = nullptr;
    }

    template <std::size_t Offset, typename... Args>
    static auto get(std::tuple<Args...>& tuple) -> std::string_view
    {
        const char* str = PyBytes_AsString(std::get<Offset + 1>(tuple));
        return {str};
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto clean(std::tuple<Args...>& tuple) -> void
    {
        if (auto* obj = std::get<Offset + 1>(tuple))
        {
            Py_DECREF(obj);
            std::get<Offset + 1>(tuple) = nullptr;
        }
    }
};

// Encoded c-string
template <typename Encoding>
struct ArgEncCStr : named_arg
{
    static_assert(detail::has_parse_ptr_value<Encoding>::value,
                  "Encoding must have a static constexpr const char* parse_ptr_value() member");

    static constexpr FmtString fmt {"et"};
    static constexpr std::size_t offset = 2;

    using value_type = detail::type_list<c_str_t>;
    using parse_type = detail::type_list<Encoding, char*>;

    template <std::size_t Offset, typename... Args>
    static constexpr void init(std::tuple<Args...>& tuple)
    {
        std::get<Offset>(tuple) = Encoding {};
        std::get<Offset + 1>(tuple) = nullptr;
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto get(std::tuple<Args...>& tuple) -> char*
    {
        return std::get<Offset + 1>(tuple);
    }

    template <std::size_t Offset, typename... Args>
    static constexpr auto clean(std::tuple<Args...>& tuple) -> void
    {
        if (auto* obj = std::get<Offset + 1>(tuple))
        {
            PyMem_Free(obj);
            std::get<Offset + 1>(tuple) = nullptr;
        }
    }
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments parser                                                         │
// └──────────────────────────────────────────────────────────────────────────┘

template <typename... Args>
struct Arguments
{
    // Types of the value tuple
    using value_types = typename detail::expand_value_types<Args...>::value_type;

    // Type of the tuple to hold the values
    using value_tuple_t = typename detail::type_list_to_tuple<value_types>::type;

    // Types to pass to Wrapped_ParseTupleAndKeywords va_list
    using parse_types = typename detail::expand_parse_types<Args...>::parse_type;

    // Type of the tuple to hold the values
    using parse_tuple_t = typename detail::type_list_to_tuple<parse_types>::type;

    // Tuple of Arguments
    using args_tuple_t =
        typename detail::type_list_to_tuple<typename detail::expand_arg_types<Args...>::type>::type;

    template <typename... Ts>
    explicit constexpr Arguments(Ts&&... args) noexcept
        : keywords {detail::build_keywords(args...)}
        , args {detail::build_named_args(args...)}
    {
        // ...
    }

    template <bool Check = true, typename Callback>
    auto match(PyObject* args, PyObject* kwArgs, Callback&& callback) const -> bool
    {
        using namespace detail;

        static_assert(is_callable_with_tuple_v<Callback, value_tuple_t>,
                      "Lambda must be callable with the expected argument "
                      "types from Arguments definition.");

        if constexpr (Check)
        {
            if ((args == nullptr || !PyTuple_Check(args))
                || (kwArgs != nullptr && !PyDict_Check(kwArgs)))
            {
                PyErr_BadInternalCall();
                return false;
            }
        }

        parse_tuple_t parsed {};
        apply_inits(parsed, &this->args);

        int result = PyArg_ParseTupleAndKeywords_Tuple(args, kwArgs, fmt.value, keywords, parsed);
        if (result)
        {
            value_tuple_t values {};
            apply_gets(parsed, values, &this->args);
            std::apply(std::forward<Callback>(callback), std::move(values));
        }

        apply_clean(parsed, &this->args);

        return result != 0;
    }

    static constexpr FmtString fmt = detail::args_fmt<Args...>;

private:
    std::array<const char*, detail::count_keywords<Args...> + 1> keywords {};
    args_tuple_t args {};
};

template <typename... Ts>
Arguments(Ts&&...) -> Arguments<Ts...>;

template <typename... ArgsAndCallbacks, std::size_t... I>
auto dispatch_overloads_impl(PyObject* args,
                             PyObject* kwArgs,
                             std::tuple<ArgsAndCallbacks...>&& args_tuple,
                             std::index_sequence<I...> _seq)
{
    return (false || ... || [&] {
        constexpr std::size_t args_idx = I * 2;
        constexpr std::size_t callback_idx = I * 2 + 1;

        auto& arguments = std::get<args_idx>(args_tuple);
        auto& callback = std::get<callback_idx>(args_tuple);

        return arguments.match(args, kwArgs, callback);
    }());
}

template <typename... ArgsAndCallbacks>
auto dispatch_overloads(PyObject* args, PyObject* kwArgs, ArgsAndCallbacks&&... args_and_callbacks)
{
    static_assert(sizeof...(args_and_callbacks) % 2 == 0,
                  "Arguments must come in pairs: Arguments object and callback");

    auto tuple = std::forward_as_tuple(args_and_callbacks...);
    constexpr std::size_t num_pairs = sizeof...(args_and_callbacks) / 2;

    return dispatch_overloads_impl(
        args, kwArgs, std::move(tuple), std::make_index_sequence<num_pairs> {});
}

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Common encodings                                                         │
// └──────────────────────────────────────────────────────────────────────────┘

struct enc_utf8
{
    static constexpr auto parse_ptr_value() { return "utf-8"; }
};

struct enc_utf16
{
    static constexpr auto parse_ptr_value() { return "utf-16"; }
};

struct enc_utf32
{
    static constexpr auto parse_ptr_value() { return "utf-32"; }
};

struct enc_ascii
{
    static constexpr auto parse_ptr_value() { return "ascii"; }
};

struct enc_latin1
{
    static constexpr auto parse_ptr_value() { return "latin1"; }
};

struct enc_iso8859
{
    static constexpr auto parse_ptr_value() { return "iso8859"; }
};

struct enc_iso8859_1
{
    static constexpr auto parse_ptr_value() { return "iso8859-1"; }
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Convenience aliases                                                      │
// └──────────────────────────────────────────────────────────────────────────┘

using arg_uchar = Arg<unsigned char>;
using arg_bool = Arg<Bool>;
using arg_short = Arg<short>;
using arg_ushort = Arg<unsigned short>;
using arg_int = Arg<int>;
using arg_uint = Arg<unsigned int>;
using arg_long = Arg<long>;
using arg_ulong = Arg<unsigned long>;
using arg_llong = Arg<long long>;
using arg_ullong = Arg<unsigned long long>;
using arg_ssize = Arg<SSize>;
using arg_1byte = Arg<Byte1>;
using arg_1char = Arg<Char1>;
using arg_float = Arg<float>;
using arg_double = Arg<double>;
using arg_cstr = Arg<c_str_t>;
using arg_string = Arg<std::string>;
using arg_string_v = Arg<std::string_view>;
using arg_pos_only = Arg<PosOnly>;
using arg_kw_only = Arg<KwOnly>;
using arg_optionals = Arg<Optional>;
using arg_object = Arg<PyObject*>;
using arg_tuple = Arg<Tuple>;
using arg_dict = Arg<Dict>;
using arg_nnbyte = Arg<NNByte>;
using arg_fspath = Arg<FSPath>;

template <typename Encoding>
using arg_enc_cstr = ArgEncCStr<Encoding>;

} // namespace Base::Py

#endif // BASE_PYARGUMENTS_H

// NOLINTEND(modernize-avoid-c-arrays)
