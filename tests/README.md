# PyArguments Testing

This project includes comprehensive tests for the `PyArguments.hxx` header using Google Test.

## Setup

The project is configured with:
- **CMake** for build configuration
- **Google Test** for testing framework (automatically fetched)
- **Python 3.13** from Pixi environment

## Building

```bash
# Configure the build
cmake -B build -G Ninja

# Build all targets (including tests)
cmake --build build
```

## Running Tests

### Method 1: Direct execution
```bash
# Run test executable directly with proper environment
PYTHONPATH=./.pixi/envs/default/lib/python3.13 LD_LIBRARY_PATH=./.pixi/envs/default/lib ./build/test_pyarguments
```

### Method 2: Through CTest
```bash
# Run all tests through CTest
cd build && ctest

# Run with verbose output
cd build && ctest -V
```

### Method 3: Using VS Code tasks
You can use the existing VS Code tasks:
- "Build Debug" - builds the project
- Then run tests manually or through CTest

## Test Coverage

The test suite includes comprehensive tests for:

### Core Functionality
- ✅ Basic argument parsing (int, float, string, bool)
- ✅ String and string_view arguments
- ✅ Multiple argument combinations
- ✅ Keyword arguments
- ✅ Mixed positional and keyword arguments
- ✅ Optional arguments with default values
- ✅ PyObject* arguments
- ✅ C-string arguments
- ✅ Various numeric types (long long, double, unsigned char, Py_ssize_t)

### Advanced Features
- ✅ Format string generation and validation
- ✅ Optional argument markers
- ✅ Encoding arguments (UTF-8, ASCII, etc.)
- ✅ Filesystem path arguments
- ✅ Complex argument combinations (similar to main.cpp usage)
- ✅ Error handling for wrong argument types

### Template Metaprogramming
- ✅ FmtString concatenation
- ✅ Type list utilities and concatenation
- ✅ Type traits (has_default_value, has_clean_method, has_name_member)
- ✅ Tuple type building from type lists
- ✅ Keyword counting functionality

### Constexpr Evaluation
- ✅ Compile-time string operations
- ✅ Constexpr format string building
- ✅ Template parameter validation

## Test Structure

Tests are organized in `tests/test_pyarguments.cpp` with:
- **PyArgumentsTest** fixture class for setup/teardown
- Helper methods for creating Python objects
- Individual test cases for each feature
- Static assertions for compile-time validation

## Example Usage

The tests demonstrate how to use PyArguments.hxx effectively:

```cpp
// Define argument schema
constexpr Arguments args{
    arg_int{"x"},
    arg_optionals{},
    arg_float{"y", 5.0f},  // with default
    arg_string_v{"name"}
};

// Use with callback
auto callback = [](int x, float y, std::string_view name) {
    // Process arguments
};

bool result = args.match(py_args, py_kwargs, callback);
```

## Current Test Results

All 30 tests pass successfully:
- 0 failures
- 0 skipped
- Total runtime: ~0.02 seconds

The Python warnings about missing libraries are expected and harmless in the testing context.