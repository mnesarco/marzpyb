// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// NOLINTBEGIN(modernize-avoid-c-arrays)

#ifndef BASE_PYARGPARSER_H
#define BASE_PYARGPARSER_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Requires:                                                                │
// │   - C++: std++20 or more                                                 │
// │   - Python 3.10+                                                         │
// │   - PyCXX 7.1+                                                           │
// └──────────────────────────────────────────────────────────────────────────┘

#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif

#ifndef PY_CXX_CONST
#define PY_CXX_CONST const
#endif

// #include "CXX/Objects.hxx"
#include <Python.h>

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Base::PyArgs Namespace                                                   │
// └──────────────────────────────────────────────────────────────────────────┘

namespace pyargs
{

/// Argument attributes
enum Flags : uint8_t
{
    Flag_None = 0,          ///< Normal argument (positional or keyword)
    Flag_PosOnly = 1u << 0, ///< Position-Only argument
    Flag_Marker = 1u << 1   ///< Control argument like | $ : ;
};

///< Control argument like | $ : ;
struct arg_marker
{
    void init_slots(std::size_t&, void**)
    {
        // Noop, but keeping the method simplifies generics
    }

    static constexpr uint8_t slots = 0;
    static constexpr Flags flags = Flag_Marker;
    static constexpr std::string_view keyword = "";
};

/// Start of optional arguments
struct arg_opt : arg_marker
{
    static constexpr std::string_view fmt = "|";
};

/// Start of keyword-only arguments
struct arg_kw_only : arg_marker
{
    static constexpr std::string_view fmt = "$";
};

/// Markers
struct mk
{
    inline static arg_opt opt {};
    inline static arg_kw_only kw_only {};
};

/// Base Argument
template <typename T>
struct BaseArg
{
    using type = T;

    T value {};
    std::string_view keyword = "";

    constexpr BaseArg() = default;
    constexpr BaseArg(const BaseArg&) = default;
    constexpr BaseArg(BaseArg&&) = default;
    constexpr BaseArg& operator=(const BaseArg&) = default;
    constexpr BaseArg& operator=(BaseArg&&) = default;

    constexpr explicit(false) BaseArg(T&& val)
        : value {std::forward<T>(val)}
    {}

    constexpr explicit(false) BaseArg(T& val)
        : value {val}
    {}

    constexpr BaseArg(std::string_view keyword, T&& val)
        : value {std::forward<T>(val)}
        , keyword {keyword}
    {}

    constexpr BaseArg(std::string_view keyword, T& val)
        : value {val}
        , keyword {keyword}
    {}

    constexpr BaseArg(std::string_view keyword)
        : value {}
        , keyword {keyword}
    {}

    constexpr explicit(false) operator T&() { return value; }
    constexpr explicit(false) operator const T&() const { return value; }

    void init_slots(std::size_t& pos, void** args) { args[pos++] = &value; }

    static constexpr uint8_t slots = 1;
    static constexpr Flags flags = Flag_None;

protected:
    // Prevent dynamic polymorphism
    constexpr ~BaseArg() = default;
};

/// Basic buffer, receives data and length
struct BufferArg : BaseArg<const char*>
{
    Py_ssize_t size {};

    constexpr BufferArg(std::string_view keyword)
        : BaseArg(keyword, "")
    {}

    constexpr operator bool() const { return value != nullptr; }

    operator const std::string_view() const
    {
        if (value)
            return std::string_view {value, static_cast<std::size_t>(size)};
        return {};
    }

    void init_slots(std::size_t& pos, void** args)
    {
        args[pos++] = &value;
        args[pos++] = &size;
    }

    static constexpr uint8_t slots = 2;
};

/// Encoded string, receive data and uses specified encoding
struct EncodedStringArg : BaseArg<char*>
{
    const char* encoding = "utf-8";

    constexpr EncodedStringArg(const char* keyword, const char* encoding)
        : BaseArg(keyword)
        , encoding {encoding}
    {}

    constexpr EncodedStringArg(const char* keyword)
        : BaseArg(keyword)
    {}

    constexpr operator const char*() const { return value; }

    void init_slots(std::size_t& pos, void** args)
    {
        args[pos++] = const_cast<char*>(encoding);
        args[pos++] = &value;
    }

    static constexpr uint8_t slots = 2;
};

/// Encoded buffer, receives data and length and uses specified encoding
struct EncodedBufferArg : BaseArg<char*>
{
    const char* encoding = "utf-8";
    Py_ssize_t size {};

    constexpr EncodedBufferArg(const char* keyword, const char* encoding)
        : BaseArg(keyword)
        , encoding {encoding}
    {}

    constexpr EncodedBufferArg(const char* keyword)
        : BaseArg(keyword)
    {}

    constexpr operator const std::string_view() const
    {
        return std::string_view {value, static_cast<std::size_t>(size)};
    }

    void init_slots(std::size_t& pos, void** args)
    {
        args[pos++] = const_cast<char*>(encoding);
        args[pos++] = &value;
        args[pos++] = &size;
    }

    static constexpr uint8_t slots = 3;
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments: Numbers                                                       │
// └──────────────────────────────────────────────────────────────────────────┘

struct arg_bool final : BaseArg<int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "p";
};

struct arg_byte final : BaseArg<unsigned char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "b";
};

struct arg_byte_noc final : BaseArg<unsigned char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "B";
};

struct arg_short final : BaseArg<short>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "h";
};

struct arg_ushort final : BaseArg<unsigned short>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "H";
};

struct arg_int final : BaseArg<int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "i";
};

struct arg_uint final : BaseArg<unsigned int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "I";
};

struct arg_long final : BaseArg<long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "l";
};

struct arg_ulong final : BaseArg<unsigned long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "k";
};

struct arg_long_long final : BaseArg<long long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "L";
};

struct arg_ulong_long final : BaseArg<unsigned long long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "K";
};

struct arg_py_size final : BaseArg<Py_ssize_t>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "n";
};

struct arg_char final : BaseArg<char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "c";
};

struct arg_utf8_char final : BaseArg<int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "C";
};

struct arg_float final : BaseArg<float>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "f";
};

struct arg_double final : BaseArg<double>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "d";
};

struct arg_complex final : BaseArg<Py_complex>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "D";
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments: Strings and Buffers                                           │
// └──────────────────────────────────────────────────────────────────────────┘

struct arg_utf8_cstr final : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "s";
};

struct arg_utf8_cstr_none final : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "z";
};

struct arg_utf8_buffer final : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "s#";
};

struct arg_bytes_buffer_none final : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "z#";
};

struct arg_utf8_pybuffer final : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "s*";
};

struct arg_utf8_pybuffer_none final : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "z*";
};

struct arg_enc_cstr final : EncodedStringArg
{
    using EncodedStringArg::EncodedStringArg;
    static constexpr std::string_view fmt = "es";
};

struct arg_enc_thru_cstr final : EncodedStringArg
{
    using EncodedStringArg::EncodedStringArg;
    static constexpr std::string_view fmt = "et";
};

struct arg_enc_buffer final : EncodedBufferArg
{
    using EncodedBufferArg::EncodedBufferArg;
    static constexpr std::string_view fmt = "es#";
};

struct arg_enc_thru_buffer final : EncodedBufferArg
{
    using EncodedBufferArg::EncodedBufferArg;
    static constexpr std::string_view fmt = "et#";
};

struct arg_cbytes final : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "y";
};

struct arg_bytes_pybuffer final : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "y*";
};

struct arg_bytes final : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "y#";
};

struct arg_pybytes final : BaseArg<PyBytesObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "S";
};

struct arg_pybyte_array final : BaseArg<PyByteArrayObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "Y";
};

struct arg_pyunicode final : BaseArg<PyObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "U";
};

struct arg_pybuffer final : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "w*";
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments: Objects                                                       │
// └──────────────────────────────────────────────────────────────────────────┘

struct arg_PyObjectAny final : BaseArg<PyObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "O";
};

struct arg_PyObject final : BaseArg<PyObject*>
{
    PyTypeObject* objectType {};

    constexpr arg_PyObject(std::string_view keyword, PyTypeObject* type, PyObject* value = nullptr)
        : BaseArg(keyword, value)
        , objectType {type}
    {}

    void init_slots(std::size_t& pos, void** args)
    {
        args[pos++] = objectType;
        args[pos++] = &value;
    }

    static constexpr std::string_view fmt = "O!";
    static constexpr uint8_t slots = 2;
};

struct Kw
{
    const char* keyword;
    Flags flags;
};

template <typename... Ts>
inline constexpr auto keywords(const Ts&... args)
{
    std::array<const char*, sizeof...(Ts) + 1> kws {};
    std::array args_kw {
        Kw {args.keyword.data(), std::decay_t<Ts>::flags}
         ...
    };

    std::size_t i = 0;
    for (auto& arg : args_kw)
    {
        if (arg.flags & Flag_Marker)
            continue;

        kws[i++] = arg.flags & Flag_PosOnly ? "" : arg.keyword;
    }

    return kws;
}

template <std::size_t SIZE, typename... Ts>
inline constexpr const char* init_format(std::array<char, SIZE>& fmt)
{
    std::array fmts {std::decay_t<Ts>::fmt...};
    std::size_t i = 0;
    for (auto& arg_fmt : fmts)
    {
        if (arg_fmt.size() > 0)
        {
            std::copy(arg_fmt.begin(), arg_fmt.end(), fmt.begin() + i);
            i += arg_fmt.size();
        }
    }
    return fmt.data();
}

template <typename... Ts>
inline const char* format()
{
    constexpr auto SIZE = (1 + ... + std::decay_t<Ts>::fmt.size());
    static std::array<char, SIZE> fmt {};
    static const char* value = init_format<SIZE, Ts...>(fmt);
    return value;
}

template <typename... Ts>
inline bool parse(PyObject* args, PyObject* kwargs, Ts&... arg)
{
    constexpr std::size_t N = (0 + ... + std::decay_t<Ts>::slots);
    std::array<void*, N> slots {};
    std::size_t pos = 0;
    void** slotsPtr = slots.data();
    (arg.init_slots(pos, slotsPtr), ...);

    auto fmt = format<Ts...>();
    auto kws = keywords(arg...);

    auto call = [&]<std::size_t... I>(const std::index_sequence<I...>&) {
        auto result = PyArg_ParseTupleAndKeywords(
            args, kwargs, fmt, const_cast<char**>(kws.data()), slots[I]...);
        return result != 0;
    };

    return call(std::make_index_sequence<N> {});
}

template <>
inline bool parse(PyObject* args, PyObject* kwargs)
{
    return PyArg_ParseTuple(args, "") != 0;
}

} // namespace pyargs
#endif