# MarzPyB

A C++ project demonstrating advanced Python C API argument parsing using modern C++17 template metaprogramming.

## Features

- **PyArguments.hxx**: Type-safe Python argument parsing library
- **Template Metaprogramming**: Compile-time format string generation
- **Modern C++17**: Constexpr, SFINAE, and variadic templates
- **Comprehensive Testing**: Google Test suite with 30+ test cases

## Project Structure

```
├── PyArguments.hxx          # Main header library
├── main.cpp                 # Dummy file, go straight to test cases
├── tests/
│   ├── test_pyarguments.cpp # Comprehensive test suite
│   └── README.md           # Testing documentation
├── CMakeLists.txt          # Build configuration
└── pixi.toml              # Python environment
```

## Building

```bash
# Configure with CMake
cmake -B build -G Ninja

# Build all targets
cmake --build build
```

## Running

```bash
# Run the test suite
cd build && ctest -V
```

## Testing

The project includes a comprehensive test suite that validates:
- All argument types and combinations
- Template metaprogramming utilities
- Compile-time string operations
- Error handling and edge cases

See [tests/README.md](tests/README.md) for detailed testing information.

## PyArguments.hxx Usage

The library provides a modern, type-safe way to parse Python arguments:

```cpp
#include "PyArguments.hxx"
using namespace Base::Py;

// Define argument schema
constexpr Arguments args{
    arg_int{"x"},
    arg_optionals{},
    arg_float{"y", 5.0f},        // with default value
    arg_string_v{"name"}
};

// Use in Python C extension function
static PyObject* my_function(PyObject* self, PyObject* args, PyObject* kwargs) {
    auto result = args.match(args, kwargs, [](int x, float y, std::string_view name) {
        // Type-safe argument processing
        std::cout << "x=" << x << ", y=" << y << ", name=" << name << std::endl;
    });

    if (!result) {
        return nullptr; // Argument parsing failed
    }

    Py_RETURN_NONE;
}
```

## Requirements

- C++17 compatible compiler (GCC 13+, Clang 10+)
- CMake 3.10+
- Python 3.10+ (via Pixi environment)
- Google Test (automatically fetched)