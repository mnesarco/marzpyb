// SPDX-License-Identifier: LGPL-2.1-or-later
// This file is solely for measuring compile time of PyArgParser.hxx
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArgParser.hxx"
#include <Python.h>

using namespace pyargs;

// ============================================================================
// Compile-time benchmark: exercises template instantiation without runtime
// ============================================================================

namespace parser_benchmark
{
#include "compile_benchmark.inc.cpp"
} // namespace parser_benchmark

// Forward declarations for other namespaces
namespace parser_benchmark2
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark2
namespace parser_benchmark3
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark3
namespace parser_benchmark4
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark4
namespace parser_benchmark5
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark5
namespace parser_benchmark6
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark6
namespace parser_benchmark7
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark7
namespace parser_benchmark8
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark8
namespace parser_benchmark9
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark9
namespace parser_benchmark10
{
void test_basic_types();
void test_numeric_types();
void test_special_types();
void test_multiple_arguments();
void test_markers();
void test_optional_with_defaults();
void test_encoding_arguments();
void test_complex_combinations();
void test_many_arguments();
void test_format_strings();
void test_type_lists();
void test_dispatch_configurations();
void test_filesystem_special();
void test_container_types();
void test_all_encodings();
void test_large_variadic();
} // namespace parser_benchmark10

int main()
{
    // Initialize Python for parse() runtime exercises
    if (!Py_IsInitialized())
        Py_Initialize();

    parser_benchmark::test_basic_types();
    parser_benchmark::test_numeric_types();
    parser_benchmark::test_special_types();
    parser_benchmark::test_multiple_arguments();
    parser_benchmark::test_markers();
    parser_benchmark::test_optional_with_defaults();
    parser_benchmark::test_encoding_arguments();
    parser_benchmark::test_complex_combinations();
    parser_benchmark::test_many_arguments();
    parser_benchmark::test_format_strings();
    parser_benchmark::test_type_lists();
    parser_benchmark::test_dispatch_configurations();
    parser_benchmark::test_filesystem_special();
    parser_benchmark::test_container_types();
    parser_benchmark::test_all_encodings();
    parser_benchmark::test_large_variadic();

    parser_benchmark2::test_basic_types();
    parser_benchmark2::test_numeric_types();
    parser_benchmark2::test_special_types();
    parser_benchmark2::test_multiple_arguments();
    parser_benchmark2::test_markers();
    parser_benchmark2::test_optional_with_defaults();
    parser_benchmark2::test_encoding_arguments();
    parser_benchmark2::test_complex_combinations();
    parser_benchmark2::test_many_arguments();
    parser_benchmark2::test_format_strings();
    parser_benchmark2::test_type_lists();
    parser_benchmark2::test_dispatch_configurations();
    parser_benchmark2::test_filesystem_special();
    parser_benchmark2::test_container_types();
    parser_benchmark2::test_all_encodings();
    parser_benchmark2::test_large_variadic();

    parser_benchmark3::test_basic_types();
    parser_benchmark3::test_numeric_types();
    parser_benchmark3::test_special_types();
    parser_benchmark3::test_multiple_arguments();
    parser_benchmark3::test_markers();
    parser_benchmark3::test_optional_with_defaults();
    parser_benchmark3::test_encoding_arguments();
    parser_benchmark3::test_complex_combinations();
    parser_benchmark3::test_many_arguments();
    parser_benchmark3::test_format_strings();
    parser_benchmark3::test_type_lists();
    parser_benchmark3::test_dispatch_configurations();
    parser_benchmark3::test_filesystem_special();
    parser_benchmark3::test_container_types();
    parser_benchmark3::test_all_encodings();
    parser_benchmark3::test_large_variadic();

    parser_benchmark4::test_basic_types();
    parser_benchmark4::test_numeric_types();
    parser_benchmark4::test_special_types();
    parser_benchmark4::test_multiple_arguments();
    parser_benchmark4::test_markers();
    parser_benchmark4::test_optional_with_defaults();
    parser_benchmark4::test_encoding_arguments();
    parser_benchmark4::test_complex_combinations();
    parser_benchmark4::test_many_arguments();
    parser_benchmark4::test_format_strings();
    parser_benchmark4::test_type_lists();
    parser_benchmark4::test_dispatch_configurations();
    parser_benchmark4::test_filesystem_special();
    parser_benchmark4::test_container_types();
    parser_benchmark4::test_all_encodings();
    parser_benchmark4::test_large_variadic();

    parser_benchmark5::test_basic_types();
    parser_benchmark5::test_numeric_types();
    parser_benchmark5::test_special_types();
    parser_benchmark5::test_multiple_arguments();
    parser_benchmark5::test_markers();
    parser_benchmark5::test_optional_with_defaults();
    parser_benchmark5::test_encoding_arguments();
    parser_benchmark5::test_complex_combinations();
    parser_benchmark5::test_many_arguments();
    parser_benchmark5::test_format_strings();
    parser_benchmark5::test_type_lists();
    parser_benchmark5::test_dispatch_configurations();
    parser_benchmark5::test_filesystem_special();
    parser_benchmark5::test_container_types();
    parser_benchmark5::test_all_encodings();
    parser_benchmark5::test_large_variadic();

    parser_benchmark6::test_basic_types();
    parser_benchmark6::test_numeric_types();
    parser_benchmark6::test_special_types();
    parser_benchmark6::test_multiple_arguments();
    parser_benchmark6::test_markers();
    parser_benchmark6::test_optional_with_defaults();
    parser_benchmark6::test_encoding_arguments();
    parser_benchmark6::test_complex_combinations();
    parser_benchmark6::test_many_arguments();
    parser_benchmark6::test_format_strings();
    parser_benchmark6::test_type_lists();
    parser_benchmark6::test_dispatch_configurations();
    parser_benchmark6::test_filesystem_special();
    parser_benchmark6::test_container_types();
    parser_benchmark6::test_all_encodings();
    parser_benchmark6::test_large_variadic();

    parser_benchmark7::test_basic_types();
    parser_benchmark7::test_numeric_types();
    parser_benchmark7::test_special_types();
    parser_benchmark7::test_multiple_arguments();
    parser_benchmark7::test_markers();
    parser_benchmark7::test_optional_with_defaults();
    parser_benchmark7::test_encoding_arguments();
    parser_benchmark7::test_complex_combinations();
    parser_benchmark7::test_many_arguments();
    parser_benchmark7::test_format_strings();
    parser_benchmark7::test_type_lists();
    parser_benchmark7::test_dispatch_configurations();
    parser_benchmark7::test_filesystem_special();
    parser_benchmark7::test_container_types();
    parser_benchmark7::test_all_encodings();
    parser_benchmark7::test_large_variadic();

    parser_benchmark8::test_basic_types();
    parser_benchmark8::test_numeric_types();
    parser_benchmark8::test_special_types();
    parser_benchmark8::test_multiple_arguments();
    parser_benchmark8::test_markers();
    parser_benchmark8::test_optional_with_defaults();
    parser_benchmark8::test_encoding_arguments();
    parser_benchmark8::test_complex_combinations();
    parser_benchmark8::test_many_arguments();
    parser_benchmark8::test_format_strings();
    parser_benchmark8::test_type_lists();
    parser_benchmark8::test_dispatch_configurations();
    parser_benchmark8::test_filesystem_special();
    parser_benchmark8::test_container_types();
    parser_benchmark8::test_all_encodings();
    parser_benchmark8::test_large_variadic();

    parser_benchmark9::test_basic_types();
    parser_benchmark9::test_numeric_types();
    parser_benchmark9::test_special_types();
    parser_benchmark9::test_multiple_arguments();
    parser_benchmark9::test_markers();
    parser_benchmark9::test_optional_with_defaults();
    parser_benchmark9::test_encoding_arguments();
    parser_benchmark9::test_complex_combinations();
    parser_benchmark9::test_many_arguments();
    parser_benchmark9::test_format_strings();
    parser_benchmark9::test_type_lists();
    parser_benchmark9::test_dispatch_configurations();
    parser_benchmark9::test_filesystem_special();
    parser_benchmark9::test_container_types();
    parser_benchmark9::test_all_encodings();
    parser_benchmark9::test_large_variadic();

    parser_benchmark10::test_basic_types();
    parser_benchmark10::test_numeric_types();
    parser_benchmark10::test_special_types();
    parser_benchmark10::test_multiple_arguments();
    parser_benchmark10::test_markers();
    parser_benchmark10::test_optional_with_defaults();
    parser_benchmark10::test_encoding_arguments();
    parser_benchmark10::test_complex_combinations();
    parser_benchmark10::test_many_arguments();
    parser_benchmark10::test_format_strings();
    parser_benchmark10::test_type_lists();
    parser_benchmark10::test_dispatch_configurations();
    parser_benchmark10::test_filesystem_special();
    parser_benchmark10::test_container_types();
    parser_benchmark10::test_all_encodings();
    parser_benchmark10::test_large_variadic();

    // Exercise parse() once (only for the first namespace to limit runtime)
    parser_benchmark::run_parse();

    if (Py_IsInitialized())
        Py_Finalize();

    return 0;
}

// NOLINTEND(modernize-use-trailing-return-type)
