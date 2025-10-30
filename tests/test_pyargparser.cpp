// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArgParser.hxx"
#include <Python.h>
#include <gtest/gtest.h>

using namespace pyargs;

class PyArgParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        if (!Py_IsInitialized())
        {
            Py_Initialize();
        }
    }

    void TearDown() override
    {
        // Don't finalize Python between tests to avoid issues
        // Py_Finalize() will be called at program exit
    }

    // Helper to create Python objects
    PyObject* createTuple(std::initializer_list<PyObject*> items)
    {
        PyObject* tuple = PyTuple_New(items.size());
        size_t i = 0;
        for (auto* item : items)
        {
            PyTuple_SET_ITEM(tuple, i++, item);
        }
        return tuple;
    }

    PyObject* createDict(std::initializer_list<std::pair<const char*, PyObject*>> items)
    {
        PyObject* dict = PyDict_New();
        for (const auto& [key, value] : items)
        {
            PyDict_SetItemString(dict, key, value);
        }
        return dict;
    }
};

// No arguments
TEST_F(PyArgParserTest, EmptyArguments)
{
    PyObject* py_args = createTuple({});
    PyObject* py_kwargs = PyDict_New();

    bool result_accept = parse(py_args, py_kwargs);

    PyObject* py_args2 = createTuple({PyFloat_FromDouble(3.14)});
    bool result_reject = !parse(py_args2, py_kwargs);

    EXPECT_TRUE(result_accept);
    EXPECT_TRUE(result_reject);

    Py_DECREF(py_args);
    Py_DECREF(py_args2);
    Py_DECREF(py_kwargs);
}

// Test mandatory int argument parsing
TEST_F(PyArgParserTest, IntArgument)
{
    arg_int x {5};

    PyObject* py_args0 = createTuple({});
    PyObject* py_args = createTuple({PyLong_FromLong(42)});
    PyObject* py_kwargs = PyDict_New();

    bool result0 = parse(py_args0, py_kwargs, x);
    bool result = parse(py_args, py_kwargs, x);

    EXPECT_FALSE(result0);
    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 42);

    Py_DECREF(py_args0);
    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test optional int argument parsing
TEST_F(PyArgParserTest, OptIntArgument)
{
    arg_int x {5};

    PyObject* py_args0 = createTuple({});
    PyObject* py_args = createTuple({PyLong_FromLong(42)});
    PyObject* py_kwargs = PyDict_New();

    bool result0 = parse(py_args0, py_kwargs, mk::opt, x);

    EXPECT_TRUE(result0);
    EXPECT_EQ(x.value, 5);

    bool result = parse(py_args, py_kwargs, mk::opt, x);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 42);

    Py_DECREF(py_args0);
    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test optional int keyword argument parsing
TEST_F(PyArgParserTest, OptIntKwArgument)
{
    arg_int x {"x", 5};

    PyObject* py_args0 = createTuple({});
    PyObject* py_kwargs0 = PyDict_New();
    PyObject* py_args = createTuple({PyLong_FromLong(42)});
    PyObject* py_kwargs = createDict({
        {"x", PyLong_FromLong(73)}
    });
    PyObject* py_kwargs1 = createDict({
        {"y", PyLong_FromLong(37)}
    });

    bool result0 = parse(py_args0, py_kwargs0, mk::opt, x);

    EXPECT_TRUE(result0);
    EXPECT_EQ(x.value, 5);

    bool result = parse(py_args, py_kwargs0, mk::opt, x);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 42);

    bool result2 = parse(py_args0, py_kwargs, x);

    EXPECT_TRUE(result2);
    EXPECT_EQ(x.value, 73);

    bool result3 = parse(py_args0, py_kwargs1, x);

    EXPECT_FALSE(result3);
    EXPECT_EQ(x.value, 73);

    Py_DECREF(py_args0);
    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
    Py_DECREF(py_kwargs0);
    Py_DECREF(py_kwargs1);
}

// Test float argument parsing
TEST_F(PyArgParserTest, FloatArgument)
{
    arg_float y;

    PyObject* py_args = createTuple({PyFloat_FromDouble(3.14)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, y);

    EXPECT_TRUE(result);
    EXPECT_FLOAT_EQ(y.value, 3.14f);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test string argument parsing
TEST_F(PyArgParserTest, StringArgument)
{
    arg_utf8_cstr text {"text"};

    PyObject* py_args = createTuple({PyUnicode_FromString("hello world")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, text);

    EXPECT_TRUE(result);
    EXPECT_STREQ(text.value, "hello world");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test string buffer argument parsing
TEST_F(PyArgParserTest, StringBufferArgument)
{
    arg_utf8_buffer text {"text"};

    PyObject* py_args = createTuple({PyUnicode_FromString("test string")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, text);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::string_view(text), "test string");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test boolean argument parsing
TEST_F(PyArgParserTest, BooleanArgument)
{
    arg_int flag {"flag"};

    PyObject* py_args = createTuple({Py_True});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, flag);

    EXPECT_TRUE(result);
    EXPECT_EQ(flag.value, 1); // True should be 1

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test multiple arguments
TEST_F(PyArgParserTest, MultipleArguments)
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_utf8_buffer name {"name"};

    PyObject* py_args
        = createTuple({PyLong_FromLong(10), PyFloat_FromDouble(2.5), PyUnicode_FromString("test")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, x, y, name);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 10);
    EXPECT_FLOAT_EQ(y.value, 2.5f);
    EXPECT_EQ(std::string_view(name), "test");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test keyword arguments
TEST_F(PyArgParserTest, KeywordArguments)
{
    arg_int x {"x"};
    arg_float y {"y"};

    PyObject* py_args = PyTuple_New(0); // Empty positional args
    PyObject* py_kwargs = createDict({
        {"x", PyLong_FromLong(15)    },
        {"y", PyFloat_FromDouble(4.2)}
    });

    bool result = parse(py_args, py_kwargs, x, y);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 15);
    EXPECT_FLOAT_EQ(y.value, 4.2f);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test mixed positional and keyword arguments
TEST_F(PyArgParserTest, MixedPositionalAndKeywordArguments)
{
    arg_int x {"x"};
    arg_float y {"y"};
    arg_utf8_buffer name {"name"};

    PyObject* py_args = createTuple({PyLong_FromLong(25)});
    PyObject* py_kwargs = createDict({
        {"y",    PyFloat_FromDouble(7.8)      },
        {"name", PyUnicode_FromString("mixed")}
    });

    bool result = parse(py_args, py_kwargs, x, y, name);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 25);
    EXPECT_FLOAT_EQ(y.value, 7.8f);
    EXPECT_EQ(std::string_view(name), "mixed");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test optional arguments with defaults
TEST_F(PyArgParserTest, OptionalArgumentsWithDefaults)
{
    arg_int x {"x"};
    arg_float y {"y", 5.0f}; // Default value

    // Only provide the required argument
    PyObject* py_args = createTuple({PyLong_FromLong(100)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, x, mk::opt, y);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 100);
    EXPECT_FLOAT_EQ(y.value, 5.0f); // Should use default value

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test PyObject* argument
TEST_F(PyArgParserTest, PyObjectArgument)
{
    arg_PyObjectAny obj {"obj"};

    PyObject* test_list = PyList_New(0);
    PyObject* py_args = createTuple({test_list});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, obj);

    EXPECT_TRUE(result);
    EXPECT_EQ(obj.value, test_list);
    EXPECT_TRUE(PyList_Check(obj.value));

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test error handling with wrong argument types
TEST_F(PyArgParserTest, ErrorHandlingWrongTypes)
{
    arg_int x {"x"};

    // Pass a string instead of an integer
    PyObject* py_args = createTuple({PyUnicode_FromString("not an integer")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, x);

    EXPECT_FALSE(result);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test c-string argument
TEST_F(PyArgParserTest, CStringArgument)
{
    arg_utf8_cstr text {"text"};

    PyObject* py_args = createTuple({PyUnicode_FromString("c-style string")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, text);

    EXPECT_TRUE(result);
    EXPECT_NE(text.value, nullptr);
    EXPECT_STREQ(text.value, "c-style string");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test Py_ssize_t argument
TEST_F(PyArgParserTest, SSizeArgument)
{
    arg_py_size size {"size"};

    PyObject* py_args = createTuple({PyLong_FromLong(12345)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, size);

    EXPECT_TRUE(result);
    EXPECT_EQ(size.value, 12345);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test long long argument
TEST_F(PyArgParserTest, LongLongArgument)
{
    arg_long_long big_num {"big_num"};

    PyObject* py_args = createTuple({PyLong_FromLongLong(9223372036854775807LL)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, big_num);

    EXPECT_TRUE(result);
    EXPECT_EQ(big_num.value, 9223372036854775807LL);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test double argument
TEST_F(PyArgParserTest, DoubleArgument)
{
    arg_double precision {"precision"};

    PyObject* py_args = createTuple({PyFloat_FromDouble(3.141592653589793)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, precision);

    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(precision.value, 3.141592653589793);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test unsigned char argument
TEST_F(PyArgParserTest, UnsignedCharArgument)
{
    arg_byte byte_val {"byte_val"};

    PyObject* py_args = createTuple({PyLong_FromLong(255)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, byte_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(byte_val.value, 255);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test encoding argument
TEST_F(PyArgParserTest, EncodingArgument)
{
    arg_enc_cstr encoded_text {"encoded_text", "utf-8"};

    PyObject* py_args = createTuple({PyUnicode_FromString("UTF-8 encoded")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, encoded_text);

    EXPECT_TRUE(result);
    EXPECT_NE(encoded_text.value, nullptr);
    EXPECT_STREQ(encoded_text.value, "UTF-8 encoded");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test short argument
TEST_F(PyArgParserTest, ShortArgument)
{
    arg_short short_val {"short_val"};

    PyObject* py_args = createTuple({PyLong_FromLong(32767)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, short_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(short_val.value, 32767);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test unsigned short argument
TEST_F(PyArgParserTest, UnsignedShortArgument)
{
    arg_ushort ushort_val {"ushort_val"};

    PyObject* py_args = createTuple({PyLong_FromLong(65535)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, ushort_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(ushort_val.value, 65535);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test unsigned int argument
TEST_F(PyArgParserTest, UnsignedIntArgument)
{
    arg_uint uint_val {"uint_val"};

    PyObject* py_args = createTuple({PyLong_FromUnsignedLong(4294967295U)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, uint_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(uint_val.value, 4294967295U);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test long argument
TEST_F(PyArgParserTest, LongArgument)
{
    arg_long long_val {"long_val"};

    PyObject* py_args = createTuple({PyLong_FromLong(2147483647L)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, long_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(long_val.value, 2147483647L);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test unsigned long argument
TEST_F(PyArgParserTest, UnsignedLongArgument)
{
    arg_ulong ulong_val {"ulong_val"};

    PyObject* py_args = createTuple({PyLong_FromUnsignedLong(4294967295UL)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, ulong_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(ulong_val.value, 4294967295UL);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test bytes argument
TEST_F(PyArgParserTest, BytesArgument)
{
    arg_bytes bytes_val {"bytes_val"};

    PyObject* py_bytes = PyBytes_FromString("binary data");
    PyObject* py_args = createTuple({py_bytes});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, bytes_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::string_view(bytes_val), "binary data");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test char argument
TEST_F(PyArgParserTest, CharArgument)
{
    arg_char char_val {"char_val"};

    PyObject* py_args = createTuple({PyBytes_FromStringAndSize("A", 1)});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, char_val);

    EXPECT_TRUE(result);
    EXPECT_EQ(char_val.value, 'A');

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test keyword-only arguments
TEST_F(PyArgParserTest, KeywordOnlyArguments)
{
    arg_int x {"x"};
    arg_int y {"y"};

    PyObject* py_args = createTuple({PyLong_FromLong(10)});
    PyObject* py_kwargs = createDict({
        {"y", PyLong_FromLong(20)}
    });

    bool result = parse(py_args, py_kwargs, x, mk::kw_only, y);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 10);
    EXPECT_EQ(y.value, 20);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test complex argument combination
TEST_F(PyArgParserTest, ComplexArgumentCombination)
{
    arg_int x {"x"};
    arg_float y {"y", 5.0f};
    arg_PyObjectAny target {"target"};
    arg_int flag {"flag"};
    arg_utf8_buffer name {"name"};
    arg_enc_cstr ux {"ux", "utf-8"};
    arg_utf8_buffer path {"path"};

    PyObject* py_args = createTuple({
        PyLong_FromLong(10),           // x
        PyFloat_FromDouble(20.5),      // y
        Py_False,                      // target
        Py_True,                       // flag
        PyUnicode_FromString("hello"), // name
        PyUnicode_FromString("UX"),    // ux
        PyUnicode_FromString("/path")  // path
    });
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, x, mk::opt, y, target, flag, name, ux, path);

    EXPECT_TRUE(result);
    EXPECT_EQ(x.value, 10);
    EXPECT_FLOAT_EQ(y.value, 20.5f);
    EXPECT_EQ(target.value, Py_False);
    EXPECT_EQ(flag.value, 1);
    EXPECT_EQ(std::string_view(name), "hello");
    EXPECT_NE(ux.value, nullptr);
    EXPECT_EQ(std::string_view(path), "/path");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test encoded buffer argument
TEST_F(PyArgParserTest, EncodedBufferArgument)
{
    arg_enc_buffer encoded_buf {"encoded_buf", "utf-8"};

    PyObject* py_args = createTuple({PyUnicode_FromString("encoded buffer")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, encoded_buf);

    EXPECT_TRUE(result);
    EXPECT_EQ(std::string_view(encoded_buf), "encoded buffer");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test null-accepting string argument
TEST_F(PyArgParserTest, NullAcceptingStringArgument)
{
    arg_utf8_cstr_none text {"text"};

    // Test with actual string
    PyObject* py_args = createTuple({PyUnicode_FromString("hello")});
    PyObject* py_kwargs = PyDict_New();

    bool result = parse(py_args, py_kwargs, text);

    EXPECT_TRUE(result);
    EXPECT_NE(text.value, nullptr);
    EXPECT_STREQ(text.value, "hello");

    // Test with None
    PyObject* py_args2 = createTuple({Py_None});
    arg_utf8_cstr_none text2 {"text"};

    bool result2 = parse(py_args2, py_kwargs, text2);

    EXPECT_TRUE(result2);
    EXPECT_EQ(text2.value, nullptr);

    Py_DECREF(py_args);
    Py_DECREF(py_args2);
    Py_DECREF(py_kwargs);
}

int main(int argc, char** argv)
{
    // Initialize Python once for all tests
    Py_Initialize();

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    // Cleanup Python
    Py_Finalize();

    return result;
}

// NOLINTEND(modernize-use-trailing-return-type)
