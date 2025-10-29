// SPDX-License-Identifier: LGPL-2.1-or-later
// This file is solely for measuring compile time of PyArgParser.hxx
// NOLINTBEGIN(modernize-use-trailing-return-type)

// Helper to touch APIs without runtime cost
inline void touch(const void*) {}

// Lightweight helpers copied from tests (no gtest)
static PyObject* bm_create_tuple(std::initializer_list<PyObject*> items)
{
    PyObject* tuple = PyTuple_New(items.size());
    size_t i = 0;
    for (auto* item : items)
    {
        PyTuple_SET_ITEM(tuple, i++, item); // steals reference
    }
    return tuple;
}

static PyObject* bm_create_dict(std::initializer_list<std::pair<const char*, PyObject*>> items)
{
    PyObject* dict = PyDict_New();
    for (const auto& [key, value] : items)
    {
        PyDict_SetItemString(dict, key, value); // borrows key, steals value ref
    }
    return dict;
}

// 1) Basic argument types
void test_basic_types()
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_double z {"z"};
    arg_char c {"c"};
    arg_utf8_char uc {"uc"};

    auto fmt = format<arg_int, arg_float, arg_double, arg_char, arg_utf8_char>();
    auto kw = keywords(x, y, z, c, uc);
    (void)fmt;
    (void)kw;
}

// 2) Numeric types
void test_numeric_types()
{
    arg_byte a {"a"};
    arg_byte_noc b {"b"};
    arg_short s {"s"};
    arg_ushort us {"us"};
    arg_uint ui {"ui"};
    arg_long l {"l"};
    arg_ulong ul {"ul"};
    arg_long_long ll {"ll"};
    arg_ulong_long ull {"ull"};
    arg_py_size psz {"psz"};
    arg_float f {"f"};
    arg_double d {"d"};
    arg_complex cx {"cx"};

    auto fmt = format<arg_byte,
                      arg_byte_noc,
                      arg_short,
                      arg_ushort,
                      arg_uint,
                      arg_long,
                      arg_ulong,
                      arg_long_long,
                      arg_ulong_long,
                      arg_py_size,
                      arg_float,
                      arg_double,
                      arg_complex>();
    auto kw = keywords(a, b, s, us, ui, l, ul, ll, ull, psz, f, d, cx);
    (void)fmt;
    (void)kw;
}

// 3) Strings and buffers
void test_special_types()
{
    arg_utf8_cstr s {"s"};
    arg_utf8_cstr_none sn {"sn"};
    arg_utf8_buffer sb {"sb"};
    arg_bytes_buffer_none zbuf {"zbuf"};
    arg_utf8_pybuffer spb {"spb"};
    arg_utf8_pybuffer_none spbn {"spbn"};

    auto fmt = format<arg_utf8_cstr,
                      arg_utf8_cstr_none,
                      arg_utf8_buffer,
                      arg_bytes_buffer_none,
                      arg_utf8_pybuffer,
                      arg_utf8_pybuffer_none>();
    auto kw = keywords(s, sn, sb, zbuf, spb, spbn);
    (void)fmt;
    (void)kw;
}

// 4) Encoded string/buffer
void test_encoding_arguments()
{
    arg_enc_cstr ecs {"ecs", "utf-8"};
    arg_enc_thru_cstr etc {"etc", "utf-8"};
    arg_enc_buffer ebuf {"ebuf", "utf-8"};
    arg_enc_thru_buffer etb {"etb", "utf-8"};

    auto fmt = format<arg_enc_cstr, arg_enc_thru_cstr, arg_enc_buffer, arg_enc_thru_buffer>();
    auto kw = keywords(ecs, etc, ebuf, etb);
    (void)fmt;
    (void)kw;
}

// 5) Object types
void test_multiple_arguments()
{
    arg_PyObjectAny any {"any"};
    arg_PyObject obj {"obj", nullptr, nullptr}; // type not set; still exercises slots order

    auto fmt = format<arg_PyObjectAny, arg_PyObject>();
    auto kw = keywords(any, obj);
    (void)fmt;
    (void)kw;
}

// 6) Markers (positionals, optionals, keyword-only)
void test_markers()
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_double z {"z"};

    auto fmt = format<arg_int, arg_opt, arg_float, arg_kw_only, arg_double>();
    auto kw = keywords(x, sep_opts(), y, sep_kw_only(), z);
    (void)fmt;
    (void)kw;
}

// 7) Optional with defaults (exercise construction)
void test_optional_with_defaults()
{
    arg_int x {"x"};
    arg_float y {"y", 1.0f};

    auto fmt = format<arg_int, arg_opt, arg_float>();
    auto kw = keywords(x, sep_opts(), y);
    (void)fmt;
    (void)kw;
}

// 8) Complex combinations
void test_complex_combinations()
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_utf8_buffer name {"name"};
    arg_PyObjectAny target {"target"};
    arg_enc_cstr enc {"enc", "utf-8"};

    auto fmt
        = format<arg_int, arg_opt, arg_float, arg_PyObjectAny, arg_utf8_buffer, arg_enc_cstr>();
    auto kw = keywords(x, sep_opts(), y, target, name, enc);
    (void)fmt;
    (void)kw;
}

// 9) Many arguments (stress)
void test_many_arguments()
{
    arg_int a1 {"a1"};
    arg_int a2 {"a2"};
    arg_int a3 {"a3"};
    arg_int a4 {"a4"};
    arg_int a5 {"a5"};
    arg_float f1 {"f1"};
    arg_float f2 {"f2"};
    arg_float f3 {"f3"};
    arg_double d1 {"d1"};
    arg_double d2 {"d2"};
    arg_utf8_cstr s1 {"s1"};
    arg_utf8_cstr s2 {"s2"};
    arg_PyObjectAny o1 {"o1"};
    arg_PyObjectAny o2 {"o2"};

    auto fmt = format<arg_int,
                      arg_int,
                      arg_int,
                      arg_int,
                      arg_int,
                      arg_float,
                      arg_float,
                      arg_float,
                      arg_double,
                      arg_double,
                      arg_utf8_cstr,
                      arg_utf8_cstr,
                      arg_PyObjectAny,
                      arg_PyObjectAny>();
    auto kw = keywords(a1, a2, a3, a4, a5, f1, f2, f3, d1, d2, s1, s2, o1, o2);
    (void)fmt;
    (void)kw;
}

// 10) Format string generation variety
void test_format_strings()
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_utf8_cstr s {"s"};
    auto fmt1 = format<arg_int, arg_float>();
    auto fmt2 = format<arg_int, arg_float, arg_utf8_cstr>();
    auto fmt3 = format<arg_int, arg_opt, arg_float>();
    (void)fmt1;
    (void)fmt2;
    (void)fmt3;
}

// 11) Type traits sanity (no runtime)
void test_type_lists()
{
    static_assert(std::is_same_v<arg_int::type, int>);
    static_assert(std::is_same_v<arg_double::type, double>);
    static_assert(arg_int::slots == 1);
    static_assert(arg_utf8_buffer::slots == 2);
}

// 12) Dispatch-like configurations (exercise different mixes)
void test_dispatch_configurations()
{
    arg_int x {"x"};
    arg_float y {"y"};
    (void)format<arg_int>();
    (void)format<arg_float>();
    (void)format<arg_int, arg_float>();
}

// 13) Filesystem-ish and bytes
void test_filesystem_special()
{
    arg_utf8_buffer path {"path"};
    arg_bytes bytes {"bytes"};
    (void)format<arg_utf8_buffer>();
    (void)format<arg_bytes>();
}

// 14) Container-like Python objects
void test_container_types()
{
    arg_pybytes pyb {"pyb"};
    arg_pybyte_array pya {"pya"};
    arg_pyunicode un {"un"};
    (void)format<arg_pybytes, arg_pybyte_array, arg_pyunicode>();
}

// 15) All encodings (aliases already covered)
void test_all_encodings()
{
    arg_enc_cstr utf8 {"u8", "utf-8"};
    arg_enc_cstr latin1 {"l1", "latin1"};
    (void)format<arg_enc_cstr, arg_enc_cstr>();
}

// 16) Large variadic instantiation
void test_large_variadic()
{
    arg_int i1 {"i1"}, i2 {"i2"}, i3 {"i3"}, i4 {"i4"};
    arg_float f1 {"f1"}, f2 {"f2"}, f3 {"f3"}, f4 {"f4"};
    arg_double d1 {"d1"}, d2 {"d2"}, d3 {"d3"}, d4 {"d4"};
    arg_utf8_cstr s1 {"s1"}, s2 {"s2"}, s3 {"s3"}, s4 {"s4"};
    (void)format<arg_int,
                 arg_int,
                 arg_int,
                 arg_int,
                 arg_float,
                 arg_float,
                 arg_float,
                 arg_float,
                 arg_double,
                 arg_double,
                 arg_double,
                 arg_double,
                 arg_utf8_cstr,
                 arg_utf8_cstr,
                 arg_utf8_cstr,
                 arg_utf8_cstr>();
}

// ==============================================================
// Runtime: exercise parse(...) without gtest
// ==============================================================
void run_parse()
{
    // EmptyArguments
    {
        PyObject* args = bm_create_tuple({});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // IntArgument
    {
        arg_int x {5};
        PyObject* args0 = bm_create_tuple({});
        PyObject* args1 = bm_create_tuple({PyLong_FromLong(42)});
        PyObject* kwargs = PyDict_New();
        (void)parse(args0, kwargs, x);
        (void)parse(args1, kwargs, x);
        Py_DECREF(args0);
        Py_DECREF(args1);
        Py_DECREF(kwargs);
    }

    // FloatArgument
    {
        arg_float y;
        PyObject* args = bm_create_tuple({PyFloat_FromDouble(3.14)});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, y);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // StringArgument
    {
        arg_utf8_cstr text {"text"};
        PyObject* args = bm_create_tuple({PyUnicode_FromString("hello world")});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, text);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // StringBufferArgument
    {
        arg_utf8_buffer text {"text"};
        PyObject* args = bm_create_tuple({PyUnicode_FromString("test string")});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, text);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // MultipleArguments + kwargs
    {
        arg_int x {"x"};
        arg_float y {"y"};
        arg_utf8_buffer name {"name"};
        PyObject* args = bm_create_tuple(
            {PyLong_FromLong(10), PyFloat_FromDouble(2.5), PyUnicode_FromString("test")});
        PyObject* kwargs = bm_create_dict({});
        (void)parse(args, kwargs, x, y, name);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // KeywordArguments
    {
        arg_int x {"x"};
        arg_float y {"y"};
        PyObject* args = PyTuple_New(0);
        PyObject* kwargs = bm_create_dict({
            {"x", PyLong_FromLong(15)    },
            {"y", PyFloat_FromDouble(4.2)}
        });
        (void)parse(args, kwargs, x, y);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // OptionalArgumentsWithDefaults
    {
        arg_int x {"x"};
        arg_float y {"y", 5.0f};
        PyObject* args = bm_create_tuple({PyLong_FromLong(100)});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, x, sep_opts(), y);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // PyObjectArgument
    {
        arg_PyObjectAny obj {"obj"};
        PyObject* lst = PyList_New(0);
        PyObject* args = bm_create_tuple({lst});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, obj);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // EncodedBufferArgument
    {
        arg_enc_buffer encoded_buf {"encoded_buf", "utf-8"};
        PyObject* args = bm_create_tuple({PyUnicode_FromString("encoded buffer")});
        PyObject* kwargs = PyDict_New();
        (void)parse(args, kwargs, encoded_buf);
        Py_DECREF(args);
        Py_DECREF(kwargs);
    }

    // NullAcceptingStringArgument
    {
        arg_utf8_cstr_none text {"text"};
        PyObject* args1 = bm_create_tuple({PyUnicode_FromString("hello")});
        PyObject* args2 = bm_create_tuple({Py_None});
        Py_INCREF(Py_None); // compensate SET_ITEM steal for singleton
        PyObject* kwargs = PyDict_New();
        (void)parse(args1, kwargs, text);
        (void)parse(args2, kwargs, text);
        Py_DECREF(args1);
        Py_DECREF(args2);
        Py_DECREF(kwargs);
    }
}

// NOLINTEND(modernize-use-trailing-return-type)
