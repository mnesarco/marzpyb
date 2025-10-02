// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArguments.hxx"
#include "tupleobject.h"
#include <Python.h>
#include <gtest/gtest.h>
#include <string>
#include <tuple>

using namespace Base::Py;
using namespace Base::Py::detail;

class PyArgumentsTest : public ::testing::Test
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

// Test FmtString concatenation
TEST_F(PyArgumentsTest, FmtStringConcatenation)
{
    constexpr auto str1 = FmtString {"hello"};
    constexpr auto str2 = FmtString {" world"};
    constexpr auto result = fmt_concat(str1, str2);

    EXPECT_STREQ(result.value, "hello world");
}

// Test type list utilities
TEST_F(PyArgumentsTest, TypeListConcatenation)
{
    using list1 = type_list<int, float>;
    using list2 = type_list<double, char>;
    using expected = type_list<int, float, double, char>;
    using result = decltype(concat_types(std::declval<list1>(), std::declval<list2>()));

    static_assert(std::is_same_v<result, expected>, "Type list concatenation failed");
}

// Test basic argument parsing with integers
TEST_F(PyArgumentsTest, BasicIntegerArgument)
{
    constexpr Arguments args {arg_int {"x"}};

    bool called = false;
    int received_value = 0;

    auto callback = [&](int x) {
        called = true;
        received_value = x;
    };

    PyObject* py_args = createTuple({PyLong_FromLong(42)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 42);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test float argument parsing
TEST_F(PyArgumentsTest, FloatArgument)
{
    constexpr Arguments args {arg_float {"y"}};

    bool called = false;
    float received_value = 0.0f;

    auto callback = [&](float y) {
        called = true;
        received_value = y;
    };

    PyObject* py_args = createTuple({PyFloat_FromDouble(3.14)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_FLOAT_EQ(received_value, 3.14f);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test string argument parsing
TEST_F(PyArgumentsTest, StringArgument)
{
    constexpr Arguments args {arg_string {"text"}};

    bool called = false;
    std::string received_value;

    auto callback = [&](const std::string& text) {
        called = true;
        received_value = text;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("hello world")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, "hello world");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test string_view argument parsing
TEST_F(PyArgumentsTest, StringViewArgument)
{
    constexpr Arguments args {arg_string_v {"text"}};

    bool called = false;
    std::string_view received_value;

    auto callback = [&](std::string_view text) {
        called = true;
        received_value = text;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("test string")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, "test string");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test boolean argument parsing
TEST_F(PyArgumentsTest, BooleanArgument)
{
    constexpr Arguments args {arg_bool {"flag"}};

    bool called = false;
    int received_value = 0;

    auto callback = [&](int flag) {
        called = true;
        received_value = flag;
    };

    PyObject* py_args = createTuple({Py_True});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 1); // True should be 1

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test multiple arguments
TEST_F(PyArgumentsTest, MultipleArguments)
{
    constexpr Arguments args {arg_int {"x"}, arg_float {"y"}, arg_string_v {"name"}};

    bool called = false;
    int received_x = 0;
    float received_y = 0.0f;
    std::string_view received_name;

    auto callback = [&](int x, float y, std::string_view name) {
        called = true;
        received_x = x;
        received_y = y;
        received_name = name;
    };

    PyObject* py_args
        = createTuple({PyLong_FromLong(10), PyFloat_FromDouble(2.5), PyUnicode_FromString("test")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_x, 10);
    EXPECT_FLOAT_EQ(received_y, 2.5f);
    EXPECT_EQ(received_name, "test");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test keyword arguments
TEST_F(PyArgumentsTest, KeywordArguments)
{
    constexpr Arguments args {arg_int {"x"}, arg_float {"y"}};

    bool called = false;
    int received_x = 0;
    float received_y = 0.0f;

    auto callback = [&](int x, float y) {
        called = true;
        received_x = x;
        received_y = y;
    };

    PyObject* py_args = PyTuple_New(0); // Empty positional args
    PyObject* py_kwargs = createDict({
        {"x", PyLong_FromLong(15)    },
        {"y", PyFloat_FromDouble(4.2)}
    });

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_x, 15);
    EXPECT_FLOAT_EQ(received_y, 4.2f);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test mixed positional and keyword arguments
TEST_F(PyArgumentsTest, MixedPositionalAndKeywordArguments)
{
    constexpr Arguments args {arg_int {"x"}, arg_float {"y"}, arg_string_v {"name"}};

    bool called = false;
    int received_x = 0;
    float received_y = 0.0f;
    std::string_view received_name;

    auto callback = [&](int x, float y, std::string_view name) {
        called = true;
        received_x = x;
        received_y = y;
        received_name = name;
    };

    PyObject* py_args = createTuple({PyLong_FromLong(25)});
    PyObject* py_kwargs = createDict({
        {"y",    PyFloat_FromDouble(7.8)      },
        {"name", PyUnicode_FromString("mixed")}
    });

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_x, 25);
    EXPECT_FLOAT_EQ(received_y, 7.8f);
    EXPECT_EQ(received_name, "mixed");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test optional arguments with defaults
TEST_F(PyArgumentsTest, OptionalArgumentsWithDefaults)
{
    constexpr Arguments args {
        arg_int {"x"},
         arg_optionals {},
         arg_float {"y", 5.0f}  // Default value
    };

    bool called = false;
    int received_x = 0;
    float received_y = 0.0f;

    auto callback = [&](int x, float y) {
        called = true;
        received_x = x;
        received_y = y;
    };

    // Only provide the required argument
    PyObject* py_args = createTuple({PyLong_FromLong(100)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_x, 100);
    EXPECT_FLOAT_EQ(received_y, 5.0f); // Should use default value

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test PyObject* argument
TEST_F(PyArgumentsTest, PyObjectArgument)
{
    constexpr Arguments args {arg_object {"obj"}};

    bool called = false;
    PyObject* received_obj = nullptr;

    auto callback = [&](PyObject* obj) {
        called = true;
        received_obj = obj;
    };

    PyObject* test_list = PyList_New(0);
    PyObject* py_args = createTuple({test_list});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_obj, test_list);
    EXPECT_TRUE(PyList_Check(received_obj));

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test error handling with wrong argument types
TEST_F(PyArgumentsTest, ErrorHandlingWrongTypes)
{
    constexpr Arguments args {arg_int {"x"}};

    bool called = false;
    auto callback = [&](int x) { called = true; };

    // Pass a string instead of an integer
    PyObject* py_args = createTuple({PyUnicode_FromString("not an integer")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_FALSE(result);
    EXPECT_FALSE(called);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test c-string argument
TEST_F(PyArgumentsTest, CStringArgument)
{
    constexpr Arguments args {arg_cstr {"text"}};

    bool called = false;
    const char* received_text = nullptr;

    auto callback = [&](const char* text) {
        called = true;
        received_text = text;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("c-style string")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_NE(received_text, nullptr);
    EXPECT_STREQ(received_text, "c-style string");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test Py_ssize_t argument
TEST_F(PyArgumentsTest, SSizeArgument)
{
    constexpr Arguments args {arg_ssize {"size"}};

    bool called = false;
    Py_ssize_t received_size = 0;

    auto callback = [&](Py_ssize_t size) {
        called = true;
        received_size = size;
    };

    PyObject* py_args = createTuple({PyLong_FromLong(12345)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_size, 12345);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test long long argument
TEST_F(PyArgumentsTest, LongLongArgument)
{
    constexpr Arguments args {arg_llong {"big_num"}};

    bool called = false;
    long long received_value = 0;

    auto callback = [&](long long big_num) {
        called = true;
        received_value = big_num;
    };

    PyObject* py_args = createTuple({PyLong_FromLongLong(9223372036854775807LL)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 9223372036854775807LL);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test double argument
TEST_F(PyArgumentsTest, DoubleArgument)
{
    constexpr Arguments args {arg_double {"precision"}};

    bool called = false;
    double received_value = 0.0;

    auto callback = [&](double precision) {
        called = true;
        received_value = precision;
    };

    PyObject* py_args = createTuple({PyFloat_FromDouble(3.141592653589793)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_DOUBLE_EQ(received_value, 3.141592653589793);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test unsigned char argument
TEST_F(PyArgumentsTest, UnsignedCharArgument)
{
    constexpr Arguments args {arg_uchar {"byte_val"}};

    bool called = false;
    unsigned char received_value = 0;

    auto callback = [&](unsigned char byte_val) {
        called = true;
        received_value = byte_val;
    };

    PyObject* py_args = createTuple({PyLong_FromLong(255)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_value, 255);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test format string generation
TEST_F(PyArgumentsTest, FormatStringGeneration)
{
    constexpr Arguments args {arg_int {"x"}, arg_float {"y"}, arg_string_v {"name"}};

    // The format string should be "if" + "s#" = "ifs#"
    EXPECT_STREQ(args.fmt.value, "ifs#");
}

// Test format string with optional marker
TEST_F(PyArgumentsTest, FormatStringWithOptional)
{
    constexpr Arguments args {arg_int {"x"}, arg_optionals {}, arg_float {"y"}};

    // The format string should be "i" + "|" + "f" = "i|f"
    EXPECT_STREQ(args.fmt.value, "i|f");
}

// Test trait has_default_value
TEST_F(PyArgumentsTest, HasDefaultValueTrait)
{
    static_assert(arg_traits<arg_float>::has_default, "arg_float should have default_value");
    static_assert(arg_traits<arg_int>::has_default, "arg_int should have default_value");
    static_assert(!arg_traits<arg_pos_only>::has_default,
                  "arg_pos_only should not have default_value");
}

// Test trait has_clean_method
TEST_F(PyArgumentsTest, HasCleanMethodTrait)
{
    static_assert(arg_traits<arg_fspath>::has_clean, "arg_fspath should have clean method");
    static_assert(!arg_traits<arg_int>::has_clean, "arg_int should not have clean method");
}

// Test encoding argument
TEST_F(PyArgumentsTest, EncodingArgument)
{
    constexpr Arguments args {arg_enc_cstr<enc_utf8> {"encoded_text"}};

    bool called = false;
    const char* received_text = nullptr;

    auto callback = [&](const char* encoded_text) {
        called = true;
        received_text = encoded_text;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("UTF-8 encoded")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_NE(received_text, nullptr);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test filesystem path argument
TEST_F(PyArgumentsTest, FilesystemPathArgument)
{
    constexpr Arguments args {arg_fspath {"path"}};

    bool called = false;
    std::string_view received_path;

    auto callback = [&](std::string_view path) {
        called = true;
        received_path = path;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("/home/user/file.txt")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test complex argument combination from main.cpp (simplified version without tuple/dict)
TEST_F(PyArgumentsTest, ComplexArgumentCombination)
{
    constexpr Arguments args {
        arg_int {"x"},
        arg_optionals {},
        arg_float {"y", 5.0f},
        arg_object {"target"},
        arg_bool {"flag"},
        arg_string_v {"name"},
        arg_enc_cstr<enc_utf8> {"ux"},
        arg_fspath {"path"}
    };

    bool called = false;
    int received_x = 0;
    float received_y = 0.0f;
    PyObject* received_target = nullptr;
    int received_flag = 0;
    std::string_view received_name;
    const char* received_ux = nullptr;
    std::string_view received_path;

    auto callback = [&](int x,
                        float y,
                        PyObject* target,
                        int flag,
                        std::string_view name,
                        const char* ux,
                        std::string_view path) {
        called = true;
        received_x = x;
        received_y = y;
        received_target = target;
        received_flag = flag;
        received_name = name;
        received_ux = ux;
        received_path = path;
    };

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

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_x, 10);
    EXPECT_FLOAT_EQ(received_y, 20.5f);
    EXPECT_EQ(received_target, Py_False);
    EXPECT_EQ(received_flag, 1);
    EXPECT_EQ(received_name, "hello");
    EXPECT_NE(received_ux, nullptr);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test count_keywords functionality
TEST_F(PyArgumentsTest, CountKeywords)
{
    constexpr size_t count1 = count_keywords<arg_int, arg_float, arg_string>;
    constexpr size_t count2 = count_keywords<arg_int, arg_pos_only, arg_float>;

    EXPECT_EQ(count1, 3); // All are named
    EXPECT_EQ(count2, 2); // pos_only is not named
}

// Test FmtString compilation
TEST_F(PyArgumentsTest, FmtStringConstexpr)
{
    constexpr auto result = fmt_concat(FmtString {"a"}, FmtString {"b"}, FmtString {"c"});
    EXPECT_STREQ(result.value, "abc");
}

// Test type traits
TEST_F(PyArgumentsTest, TypeTraits)
{
    using argument_int = arg_traits<arg_int>;
    using raw_int = arg_traits<int>;
    static_assert(argument_int::has_name, "arg_int should have name member");
    static_assert(!raw_int::has_name, "int should not have name member");
}

// Test encoding names
TEST_F(PyArgumentsTest, EncodingNames)
{
    EXPECT_STREQ(enc_utf8::parse_ptr_value(), "utf-8");
    EXPECT_STREQ(enc_utf16::parse_ptr_value(), "utf-16");
    EXPECT_STREQ(enc_ascii::parse_ptr_value(), "ascii");
    EXPECT_STREQ(enc_latin1::parse_ptr_value(), "latin1");
}

// Test tuple type building
TEST_F(PyArgumentsTest, TupleTypeBuilding)
{
    using test_list = type_list<int, float, std::string>;
    using tuple_type = typename type_list_to_tuple<test_list>::type;
    using expected_type = std::tuple<int, float, std::string>;

    static_assert(std::is_same_v<tuple_type, expected_type>, "Tuple type building failed");
}

// Test dispatch_overloads with multiple signatures - middle one should match
TEST_F(PyArgumentsTest, DispatchOverloadsMultipleSignatures)
{
    // Define three different argument signatures
    constexpr Arguments args1 {arg_string {"text"}, arg_float {"value"}}; // First signature
    constexpr Arguments args2 {arg_int {"number"},
                               arg_bool {"flag"}}; // Second signature (will match)
    constexpr Arguments args3 {arg_double {"precision"}, arg_cstr {"name"}}; // Third signature

    // Track which callbacks were called
    bool callback1_called = false;
    bool callback2_called = false;
    bool callback3_called = false;

    // Data to verify correct callback was invoked
    std::string received_text;
    float received_value = 0.0f;
    int received_number = 0;
    int received_flag = 0;
    double received_precision = 0.0;
    const char* received_name = nullptr;

    // Define callbacks for each signature
    auto callback1 = [&](const std::string& text, float value) {
        callback1_called = true;
        received_text = text;
        received_value = value;
    };

    auto callback2 = [&](int number, int flag) {
        callback2_called = true;
        received_number = number;
        received_flag = flag;
    };

    auto callback3 = [&](double precision, const char* name) {
        callback3_called = true;
        received_precision = precision;
        received_name = name;
    };

    // Create Python arguments that match the second signature (int, bool)
    PyObject* py_args = createTuple({PyLong_FromLong(42), Py_True});
    PyObject* py_kwargs = PyDict_New();

    // Call dispatch_overloads with all three argument/callback pairs
    bool result = dispatch_overloads(py_args,
                                     py_kwargs,
                                     args1,
                                     callback1, // First pair (won't match)
                                     args2,
                                     callback2, // Second pair (should match)
                                     args3,
                                     callback3); // Third pair (won't match)

    // Verify that only the middle (second) callback was called
    EXPECT_TRUE(result);
    EXPECT_FALSE(callback1_called);
    EXPECT_TRUE(callback2_called);
    EXPECT_FALSE(callback3_called);

    // Verify the correct values were received
    EXPECT_EQ(received_number, 42);
    EXPECT_EQ(received_flag, 1); // Python True -> 1

    // Verify other callbacks weren't touched
    EXPECT_EQ(received_text, "");
    EXPECT_EQ(received_value, 0.0f);
    EXPECT_EQ(received_precision, 0.0);
    EXPECT_EQ(received_name, nullptr);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test dispatch_overloads with no matching signature
TEST_F(PyArgumentsTest, DispatchOverloadsNoMatch)
{
    // Define argument signatures that won't match the input
    constexpr Arguments args1 {arg_string {"text"}};
    constexpr Arguments args2 {arg_double {"precision"}};
    constexpr Arguments args3 {arg_cstr {"name"}};

    bool callback1_called = false;
    bool callback2_called = false;
    bool callback3_called = false;

    auto callback1 = [&](const std::string& text) { callback1_called = true; };
    auto callback2 = [&](double precision) { callback2_called = true; };
    auto callback3 = [&](const char* name) { callback3_called = true; };

    // Create Python arguments that don't match any signature (pass an int)
    PyObject* py_tuple = PyTuple_New(0);
    PyObject* py_args = createTuple({py_tuple});
    PyObject* py_kwargs = PyDict_New();

    bool result = dispatch_overloads(
        py_args, py_kwargs, args1, callback1, args2, callback2, args3, callback3);

    // Should return false and no callbacks should be called
    EXPECT_FALSE(result);
    EXPECT_FALSE(callback1_called);
    EXPECT_FALSE(callback2_called);
    EXPECT_FALSE(callback3_called);

    Py_DECREF(py_tuple);
    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// Test dispatch_overloads with keyword arguments
TEST_F(PyArgumentsTest, DispatchOverloadsWithKeywords)
{
    // Define signatures with different keyword argument patterns
    constexpr Arguments args1 {arg_int {"x"}, arg_string {"text"}};
    constexpr Arguments args2 {arg_float {"value"}, arg_bool {"enabled"}};
    constexpr Arguments args3 {arg_string {"message"}, arg_int {"count"}};

    bool callback1_called = false;
    bool callback2_called = false;
    bool callback3_called = false;

    float received_value = 0.0f;
    int received_enabled = 0;

    auto callback1 = [&](int x, const std::string& text) { callback1_called = true; };
    auto callback2 = [&](float value, int enabled) {
        callback2_called = true;
        received_value = value;
        received_enabled = enabled;
    };
    auto callback3 = [&](const std::string& message, int count) { callback3_called = true; };

    // Create keyword arguments that match the second signature
    PyObject* py_args = PyTuple_New(0); // No positional args
    PyObject* py_kwargs = createDict({
        {"value",   PyFloat_FromDouble(3.14)},
        {"enabled", Py_False                }
    });

    bool result = dispatch_overloads(
        py_args, py_kwargs, args1, callback1, args2, callback2, args3, callback3);

    EXPECT_TRUE(result);
    EXPECT_FALSE(callback1_called);
    EXPECT_TRUE(callback2_called);
    EXPECT_FALSE(callback3_called);

    EXPECT_FLOAT_EQ(received_value, 3.14f);
    EXPECT_EQ(received_enabled, 0); // Python False -> 0

    Py_DECREF(py_args);
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
