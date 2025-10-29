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

enum Flags : uint8_t
{
    Flag_None = 0,
    Flag_PosOnly = 1u << 0,
    Flag_Marker = 1u << 1
};

struct arg_marker
{
    template <std::size_t N>
    void init_slots(std::size_t&, std::array<void*, N>&)
    {}

    static constexpr uint8_t slots = 0;
    static constexpr Flags flags = Flag_Marker;
    static constexpr std::string_view keyword = "";
};

struct arg_opt : arg_marker
{
    static constexpr std::string_view fmt = "|";
};

struct arg_kw_only : arg_marker
{
    static constexpr std::string_view fmt = "$";
};

inline arg_opt& sep_opts()
{
    static arg_opt value {};
    return value;
}

inline arg_kw_only& sep_kw_only()
{
    static arg_kw_only value {};
    return value;
}

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
    constexpr ~BaseArg() = default;

    constexpr explicit(false) BaseArg(T&& val)
        : value {std::forward<T>(val)}
    {}

    constexpr BaseArg(std::string_view keyword, T&& val)
        : value {std::forward<T>(val)}
        , keyword {keyword}
    {}

    constexpr explicit(false) BaseArg(T& val)
        : value {val}
    {}

    constexpr BaseArg(std::string_view keyword, T& val)
        : value {val}
        , keyword {keyword}
    {}

    constexpr BaseArg(std::string_view keyword)
        : value {}
        , keyword {keyword}
    {}

    template <std::size_t N>
    void init_slots(std::size_t& pos, std::array<void*, N>& args)
    {
        args[pos++] = &value;
    }

    static constexpr uint8_t slots = 1;
    static constexpr Flags flags = Flag_None;
    static constexpr std::string_view fmt = "";
};

struct BufferArg : BaseArg<const char*>
{
    Py_ssize_t size {};

    BufferArg(std::string_view keyword)
        : BaseArg(keyword, "")
    {}

    operator bool() const { return value != nullptr; }

    operator std::string_view() const
    {
        if (value)
            return std::string_view {value, static_cast<std::size_t>(size)};
        return {};
    }

    template <std::size_t N>
    void init_slots(std::size_t& pos, std::array<void*, N>& args)
    {
        args[pos++] = &value;
        args[pos++] = &size;
    }

    static constexpr uint8_t slots = 2;
};

struct EncodedStringArg : BaseArg<char*>
{
    const char* encoding = "utf-8";

    EncodedStringArg(const char* keyword, const char* encoding)
        : BaseArg(keyword)
        , encoding {encoding}
    {}

    EncodedStringArg(const char* keyword)
        : BaseArg(keyword)
    {}

    operator const char*() const { return value; }

    template <std::size_t N>
    void init_slots(std::size_t& pos, std::array<void*, N>& args)
    {
        args[pos++] = const_cast<char*>(encoding);
        args[pos++] = &value;
    }

    static constexpr uint8_t slots = 2;
};

struct EncodedBufferArg : BaseArg<char*>
{
    const char* encoding = "utf-8";
    Py_ssize_t size {};

    EncodedBufferArg(const char* keyword, const char* encoding)
        : BaseArg(keyword)
        , encoding {encoding}
    {}

    EncodedBufferArg(const char* keyword)
        : BaseArg(keyword)
    {}

    operator std::string_view() const
    {
        return std::string_view {value, static_cast<std::size_t>(size)};
    }

    template <std::size_t N>
    void init_slots(std::size_t& pos, std::array<void*, N>& args)
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

struct arg_byte : BaseArg<unsigned char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "b";
};

struct arg_byte_noc : BaseArg<unsigned char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "B";
};

struct arg_short : BaseArg<short>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "h";
};

struct arg_ushort : BaseArg<unsigned short>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "H";
};

struct arg_int : BaseArg<int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "i";
};

struct arg_uint : BaseArg<unsigned int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "I";
};

struct arg_long : BaseArg<long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "l";
};

struct arg_ulong : BaseArg<unsigned long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "k";
};

struct arg_long_long : BaseArg<long long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "L";
};

struct arg_ulong_long : BaseArg<unsigned long long>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "K";
};

struct arg_py_size : BaseArg<Py_ssize_t>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "n";
};

struct arg_char : BaseArg<char>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "c";
};

struct arg_utf8_char : BaseArg<int>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "C";
};

struct arg_float : BaseArg<float>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "f";
};

struct arg_double : BaseArg<double>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "d";
};

struct arg_complex : BaseArg<Py_complex>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "D";
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments: Strings and Buffers                                           │
// └──────────────────────────────────────────────────────────────────────────┘

struct arg_utf8_cstr : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "s";
};

struct arg_utf8_cstr_none : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "z";
};

struct arg_utf8_buffer : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "s#";
};

struct arg_bytes_buffer_none : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "z#";
};

struct arg_utf8_pybuffer : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "s*";
};

struct arg_utf8_pybuffer_none : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "z*";
};

struct arg_enc_cstr : EncodedStringArg
{
    using EncodedStringArg::EncodedStringArg;
    static constexpr std::string_view fmt = "es";
};

struct arg_enc_thru_cstr : EncodedStringArg
{
    using EncodedStringArg::EncodedStringArg;
    static constexpr std::string_view fmt = "et";
};

struct arg_enc_buffer : EncodedBufferArg
{
    using EncodedBufferArg::EncodedBufferArg;
    static constexpr std::string_view fmt = "es#";
};

struct arg_enc_thru_buffer : EncodedBufferArg
{
    using EncodedBufferArg::EncodedBufferArg;
    static constexpr std::string_view fmt = "et#";
};

struct arg_cbytes : BaseArg<const char*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "y";
};

struct arg_bytes_pybuffer : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "y*";
};

struct arg_bytes : BufferArg
{
    using BufferArg::BufferArg;
    static constexpr std::string_view fmt = "y#";
};

struct arg_pybytes : BaseArg<PyBytesObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "S";
};

struct arg_pybyte_array : BaseArg<PyByteArrayObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "Y";
};

struct arg_pyunicode : BaseArg<PyObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "U";
};

struct arg_pybuffer : BaseArg<Py_buffer>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "w*";
};

// ┌──────────────────────────────────────────────────────────────────────────┐
// │ Arguments: Objects                                                       │
// └──────────────────────────────────────────────────────────────────────────┘

struct arg_PyObjectAny : BaseArg<PyObject*>
{
    using BaseArg::BaseArg;
    static constexpr std::string_view fmt = "O";
};

struct arg_PyObject : BaseArg<PyObject*>
{
    PyTypeObject* objectType {};

    arg_PyObject(std::string_view keyword, PyTypeObject* type, PyObject* value = nullptr)
        : BaseArg(keyword, value)
        , objectType {type}
    {}

    template <std::size_t N>
    void init_slots(std::size_t& pos, std::array<void*, N>& args)
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
inline auto keywords(const Ts&... args)
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
    (arg.init_slots(pos, slots), ...);

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