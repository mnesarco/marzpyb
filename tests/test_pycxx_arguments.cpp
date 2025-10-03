// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// Test file for PyCXX argument types
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArguments.hxx"
#include "CXX/Python3/Objects.hxx"
#include <Python.h>
#include <gtest/gtest.h>
#include <string>

using namespace Base::Py;
namespace cxx = ::Py;

class PyCxxArgumentsTest : public ::testing::Test
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

    PyObject* createList(std::initializer_list<PyObject*> items)
    {
        PyObject* list = PyList_New(items.size());
        size_t i = 0;
        for (auto* item : items)
        {
            PyList_SET_ITEM(list, i++, item);
        }
        return list;
    }
};

// ============================================================================
// Test PyCXX::Object argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, ObjectArgument)
{
    constexpr Arguments args {arg_Object {"obj"}};

    bool called = false;
    cxx::Object received_obj;

    auto callback = [&](cxx::Object obj) {
        called = true;
        received_obj = obj;
    };

    PyObject* test_string = PyUnicode_FromString("test");
    PyObject* py_args = createTuple({test_string});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::String argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, StringArgument)
{
    constexpr Arguments args {arg_String {"text"}};

    bool called = false;
    cxx::String received_str;

    auto callback = [&](cxx::String text) {
        called = true;
        received_str = text;
    };

    PyObject* py_args = createTuple({PyUnicode_FromString("Hello PyCXX")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(std::string(received_str.as_std_string()), "Hello PyCXX");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Long argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, LongArgument)
{
    constexpr Arguments args {arg_long {"number"}};

    bool called = false;
    long received_long;

    auto callback = [&](long number) {
        called = true;
        received_long = number;
    };

    PyObject* py_args = createTuple({PyLong_FromLong(42)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_long, 42);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Float argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, FloatArgument)
{
    constexpr Arguments args {arg_double {"value"}};

    bool called = false;
    double received_float;

    auto callback = [&](double value) {
        called = true;
        received_float = value;
    };

    PyObject* py_args = createTuple({PyFloat_FromDouble(3.14)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_DOUBLE_EQ(received_float, 3.14);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Tuple argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, TupleArgument)
{
    constexpr Arguments args {arg_Tuple {"data"}};

    bool called = false;
    cxx::Tuple received_tuple;

    auto callback = [&](cxx::Tuple data) {
        called = true;
        received_tuple = data;
    };

    PyObject* test_tuple
        = createTuple({PyLong_FromLong(1), PyLong_FromLong(2), PyLong_FromLong(3)});
    PyObject* py_args = createTuple({test_tuple});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_tuple.size(), 3);
    EXPECT_EQ(static_cast<long>(cxx::Long(received_tuple[0])), 1);
    EXPECT_EQ(static_cast<long>(cxx::Long(received_tuple[1])), 2);
    EXPECT_EQ(static_cast<long>(cxx::Long(received_tuple[2])), 3);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::List argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, ListArgument)
{
    constexpr Arguments args {arg_List {"items"}};

    bool called = false;
    cxx::List received_list;

    auto callback = [&](cxx::List items) {
        called = true;
        received_list = items;
    };

    PyObject* test_list = createList(
        {PyUnicode_FromString("a"), PyUnicode_FromString("b"), PyUnicode_FromString("c")});
    PyObject* py_args = createTuple({test_list});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_list.size(), 3);
    EXPECT_EQ(std::string(cxx::String(received_list[0]).as_std_string()), "a");
    EXPECT_EQ(std::string(cxx::String(received_list[1]).as_std_string()), "b");
    EXPECT_EQ(std::string(cxx::String(received_list[2]).as_std_string()), "c");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Dict argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, DictArgument)
{
    constexpr Arguments args {arg_Dict {"mapping"}};

    bool called = false;
    cxx::Dict received_dict;

    auto callback = [&](cxx::Dict mapping) {
        called = true;
        received_dict = mapping;
    };

    PyObject* test_dict = createDict({
        {"key1", PyLong_FromLong(100)         },
        {"key2", PyUnicode_FromString("value")}
    });
    PyObject* py_args = createTuple({test_dict});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_dict.size(), 2);
    EXPECT_TRUE(received_dict.hasKey("key1"));
    EXPECT_TRUE(received_dict.hasKey("key2"));
    EXPECT_EQ(static_cast<long>(cxx::Long(received_dict["key1"])), 100);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Callable argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, CallableArgument)
{
    constexpr Arguments args {arg_Callable {"func"}};

    bool called = false;
    cxx::Callable received_callable;

    auto callback = [&](cxx::Callable func) {
        called = true;
        received_callable = func;
    };

    // Create a simple lambda function in Python
    PyObject* py_func = PyRun_String(
        "lambda x: x * 2", Py_eval_input, PyEval_GetBuiltins(), PyEval_GetBuiltins());

    PyObject* py_args = createTuple({py_func});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_TRUE(received_callable.isCallable());

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX::Bytes argument
// ============================================================================

TEST_F(PyCxxArgumentsTest, BytesArgument)
{
    constexpr Arguments args {arg_Bytes {"data"}};

    bool called = false;
    cxx::Bytes received_bytes;

    auto callback = [&](cxx::Bytes data) {
        called = true;
        received_bytes = data;
    };

    const char* test_data = "binary data";
    PyObject* py_args = createTuple({PyBytes_FromString(test_data)});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_bytes.size(), strlen(test_data));

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test Multiple PyCXX arguments
// ============================================================================

TEST_F(PyCxxArgumentsTest, MultiplePyCxxArguments)
{
    constexpr Arguments args {
        arg_String {"name"}, arg_long {"count"}, arg_List {"items"}, arg_Dict {"config"}};

    bool called = false;
    cxx::String received_name;
    long received_count;
    cxx::List received_items;
    cxx::Dict received_config;

    auto callback = [&](cxx::String name, long count, cxx::List items, cxx::Dict config) {
        called = true;
        received_name = name;
        received_count = count;
        received_items = items;
        received_config = config;
    };

    PyObject* py_args = createTuple({
        PyUnicode_FromString("test"),
        PyLong_FromLong(5),
        createList({PyLong_FromLong(1), PyLong_FromLong(2)}
        ),
        createDict({{"enabled", Py_True}}
        )
    });
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(std::string(received_name.as_std_string()), "test");
    EXPECT_EQ(received_count, 5);
    EXPECT_EQ(received_items.size(), 2);
    EXPECT_EQ(received_config.size(), 1);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX with keyword arguments
// ============================================================================

TEST_F(PyCxxArgumentsTest, PyCxxWithKeywords)
{
    constexpr Arguments args {arg_String {"text"}, arg_long {"number"}};

    bool called = false;
    cxx::String received_text;
    long received_number;

    auto callback = [&](cxx::String text, long number) {
        called = true;
        received_text = text;
        received_number = number;
    };

    PyObject* py_args = PyTuple_New(0); // Empty positional args
    PyObject* py_kwargs = createDict({
        {"text",   PyUnicode_FromString("keyword")},
        {"number", PyLong_FromLong(99)            }
    });

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(std::string(received_text.as_std_string()), "keyword");
    EXPECT_EQ(received_number, 99);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test PyCXX with optional arguments
// ============================================================================

TEST_F(PyCxxArgumentsTest, PyCxxWithOptionals)
{
    constexpr Arguments args {arg_String {"name"}, arg_optionals {}, arg_long {"count"}};

    bool called = false;
    cxx::String received_name;
    long received_count;

    auto callback = [&](cxx::String name, long count) {
        called = true;
        received_name = name;
        received_count = count;
    };

    // Only provide required argument
    PyObject* py_args = createTuple({PyUnicode_FromString("required")});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(std::string(received_name.as_std_string()), "required");

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test Mixed PyCXX and native types
// ============================================================================

TEST_F(PyCxxArgumentsTest, MixedPyCxxAndNativeTypes)
{
    constexpr Arguments args {
        arg_int {"id"}, arg_String {"name"}, arg_float {"score"}, arg_List {"tags"}};

    bool called = false;
    int received_id = 0;
    cxx::String received_name;
    float received_score = 0.0f;
    cxx::List received_tags;

    auto callback = [&](int id, cxx::String name, float score, cxx::List tags) {
        called = true;
        received_id = id;
        received_name = name;
        received_score = score;
        received_tags = tags;
    };

    PyObject* py_args
        = createTuple({PyLong_FromLong(123),
                       PyUnicode_FromString("mixed"),
                       PyFloat_FromDouble(95.5),
                       createList({PyUnicode_FromString("tag1"), PyUnicode_FromString("tag2")})});
    PyObject* py_kwargs = PyDict_New();

    bool result = args.match(py_args, py_kwargs, callback);

    EXPECT_TRUE(result);
    EXPECT_TRUE(called);
    EXPECT_EQ(received_id, 123);
    EXPECT_EQ(std::string(received_name.as_std_string()), "mixed");
    EXPECT_FLOAT_EQ(received_score, 95.5f);
    EXPECT_EQ(received_tags.size(), 2);

    Py_DECREF(py_args);
    Py_DECREF(py_kwargs);
}

// ============================================================================
// Test dispatch_overloads with PyCXX types
// ============================================================================

TEST_F(PyCxxArgumentsTest, DispatchOverloadsWithPyCxx)
{
    constexpr Arguments args1 {arg_Tuple {"tuple"}};
    constexpr Arguments args2 {arg_long {"number"}};
    constexpr Arguments args3 {arg_List {"items"}};

    bool callback1_called = false;
    bool callback2_called = false;
    bool callback3_called = false;

    long received_number = 0;

    auto callback1 = [&](cxx::Tuple tuple) { callback1_called = true; };
    auto callback2 = [&](long number) {
        callback2_called = true;
        received_number = number;
    };
    auto callback3 = [&](cxx::List items) { callback3_called = true; };

    // Create args that match second signature
    PyObject* py_args = createTuple({PyLong_FromLong(777)});
    PyObject* py_kwargs = PyDict_New();

    bool result = dispatch_overloads(
        py_args, py_kwargs, args1, callback1, args2, callback2, args3, callback3);

    EXPECT_TRUE(result);
    EXPECT_FALSE(callback1_called);
    EXPECT_TRUE(callback2_called);
    EXPECT_FALSE(callback3_called);
    EXPECT_EQ(received_number, 777);

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
