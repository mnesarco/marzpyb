// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright 2025 Frank David Martínez M. <mnesarco>
// This file is solely for measuring compile time of PyArguments.hxx
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArguments.hxx"
#include <Python.h>
#include <string>
#include <tuple>

using namespace Base::Py;

// ============================================================================
// Compile-time benchmark: exercises template instantiation without runtime
// ============================================================================

namespace compile_benchmark
{
#include "compile_benchmark.inc.cpp"
} // namespace compile_benchmark

// Forward declarations for other benchmark namespaces
namespace compile_benchmark2
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
} // namespace compile_benchmark2

namespace compile_benchmark3
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
} // namespace compile_benchmark3

namespace compile_benchmark4
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
} // namespace compile_benchmark4

namespace compile_benchmark5
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
} // namespace compile_benchmark5

namespace compile_benchmark6
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
} // namespace compile_benchmark6

namespace compile_benchmark7
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
} // namespace compile_benchmark7

namespace compile_benchmark8
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
} // namespace compile_benchmark8

namespace compile_benchmark9
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
} // namespace compile_benchmark9

namespace compile_benchmark10
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
} // namespace compile_benchmark10

// ============================================================================
// Main function - minimal Python initialization
// ============================================================================

int main()
{
    // Call all test functions from all namespaces to force template instantiation
    // Namespace 1
    compile_benchmark::test_basic_types();
    compile_benchmark::test_numeric_types();
    compile_benchmark::test_special_types();
    compile_benchmark::test_multiple_arguments();
    compile_benchmark::test_markers();
    compile_benchmark::test_optional_with_defaults();
    compile_benchmark::test_encoding_arguments();
    compile_benchmark::test_complex_combinations();
    compile_benchmark::test_many_arguments();
    compile_benchmark::test_format_strings();
    compile_benchmark::test_type_lists();
    compile_benchmark::test_dispatch_configurations();
    compile_benchmark::test_filesystem_special();
    compile_benchmark::test_container_types();
    compile_benchmark::test_all_encodings();
    compile_benchmark::test_large_variadic();

    // Namespace 2
    compile_benchmark2::test_basic_types();
    compile_benchmark2::test_numeric_types();
    compile_benchmark2::test_special_types();
    compile_benchmark2::test_multiple_arguments();
    compile_benchmark2::test_markers();
    compile_benchmark2::test_optional_with_defaults();
    compile_benchmark2::test_encoding_arguments();
    compile_benchmark2::test_complex_combinations();
    compile_benchmark2::test_many_arguments();
    compile_benchmark2::test_format_strings();
    compile_benchmark2::test_type_lists();
    compile_benchmark2::test_dispatch_configurations();
    compile_benchmark2::test_filesystem_special();
    compile_benchmark2::test_container_types();
    compile_benchmark2::test_all_encodings();
    compile_benchmark2::test_large_variadic();

    // Namespace 3
    compile_benchmark3::test_basic_types();
    compile_benchmark3::test_numeric_types();
    compile_benchmark3::test_special_types();
    compile_benchmark3::test_multiple_arguments();
    compile_benchmark3::test_markers();
    compile_benchmark3::test_optional_with_defaults();
    compile_benchmark3::test_encoding_arguments();
    compile_benchmark3::test_complex_combinations();
    compile_benchmark3::test_many_arguments();
    compile_benchmark3::test_format_strings();
    compile_benchmark3::test_type_lists();
    compile_benchmark3::test_dispatch_configurations();
    compile_benchmark3::test_filesystem_special();
    compile_benchmark3::test_container_types();
    compile_benchmark3::test_all_encodings();
    compile_benchmark3::test_large_variadic();

    // Namespace 4
    compile_benchmark4::test_basic_types();
    compile_benchmark4::test_numeric_types();
    compile_benchmark4::test_special_types();
    compile_benchmark4::test_multiple_arguments();
    compile_benchmark4::test_markers();
    compile_benchmark4::test_optional_with_defaults();
    compile_benchmark4::test_encoding_arguments();
    compile_benchmark4::test_complex_combinations();
    compile_benchmark4::test_many_arguments();
    compile_benchmark4::test_format_strings();
    compile_benchmark4::test_type_lists();
    compile_benchmark4::test_dispatch_configurations();
    compile_benchmark4::test_filesystem_special();
    compile_benchmark4::test_container_types();
    compile_benchmark4::test_all_encodings();
    compile_benchmark4::test_large_variadic();

    // Namespace 5
    compile_benchmark5::test_basic_types();
    compile_benchmark5::test_numeric_types();
    compile_benchmark5::test_special_types();
    compile_benchmark5::test_multiple_arguments();
    compile_benchmark5::test_markers();
    compile_benchmark5::test_optional_with_defaults();
    compile_benchmark5::test_encoding_arguments();
    compile_benchmark5::test_complex_combinations();
    compile_benchmark5::test_many_arguments();
    compile_benchmark5::test_format_strings();
    compile_benchmark5::test_type_lists();
    compile_benchmark5::test_dispatch_configurations();
    compile_benchmark5::test_filesystem_special();
    compile_benchmark5::test_container_types();
    compile_benchmark5::test_all_encodings();
    compile_benchmark5::test_large_variadic();

    // Namespace 6
    compile_benchmark6::test_basic_types();
    compile_benchmark6::test_numeric_types();
    compile_benchmark6::test_special_types();
    compile_benchmark6::test_multiple_arguments();
    compile_benchmark6::test_markers();
    compile_benchmark6::test_optional_with_defaults();
    compile_benchmark6::test_encoding_arguments();
    compile_benchmark6::test_complex_combinations();
    compile_benchmark6::test_many_arguments();
    compile_benchmark6::test_format_strings();
    compile_benchmark6::test_type_lists();
    compile_benchmark6::test_dispatch_configurations();
    compile_benchmark6::test_filesystem_special();
    compile_benchmark6::test_container_types();
    compile_benchmark6::test_all_encodings();
    compile_benchmark6::test_large_variadic();

    // Namespace 7
    compile_benchmark7::test_basic_types();
    compile_benchmark7::test_numeric_types();
    compile_benchmark7::test_special_types();
    compile_benchmark7::test_multiple_arguments();
    compile_benchmark7::test_markers();
    compile_benchmark7::test_optional_with_defaults();
    compile_benchmark7::test_encoding_arguments();
    compile_benchmark7::test_complex_combinations();
    compile_benchmark7::test_many_arguments();
    compile_benchmark7::test_format_strings();
    compile_benchmark7::test_type_lists();
    compile_benchmark7::test_dispatch_configurations();
    compile_benchmark7::test_filesystem_special();
    compile_benchmark7::test_container_types();
    compile_benchmark7::test_all_encodings();
    compile_benchmark7::test_large_variadic();

    // Namespace 8
    compile_benchmark8::test_basic_types();
    compile_benchmark8::test_numeric_types();
    compile_benchmark8::test_special_types();
    compile_benchmark8::test_multiple_arguments();
    compile_benchmark8::test_markers();
    compile_benchmark8::test_optional_with_defaults();
    compile_benchmark8::test_encoding_arguments();
    compile_benchmark8::test_complex_combinations();
    compile_benchmark8::test_many_arguments();
    compile_benchmark8::test_format_strings();
    compile_benchmark8::test_type_lists();
    compile_benchmark8::test_dispatch_configurations();
    compile_benchmark8::test_filesystem_special();
    compile_benchmark8::test_container_types();
    compile_benchmark8::test_all_encodings();
    compile_benchmark8::test_large_variadic();

    // Namespace 9
    compile_benchmark9::test_basic_types();
    compile_benchmark9::test_numeric_types();
    compile_benchmark9::test_special_types();
    compile_benchmark9::test_multiple_arguments();
    compile_benchmark9::test_markers();
    compile_benchmark9::test_optional_with_defaults();
    compile_benchmark9::test_encoding_arguments();
    compile_benchmark9::test_complex_combinations();
    compile_benchmark9::test_many_arguments();
    compile_benchmark9::test_format_strings();
    compile_benchmark9::test_type_lists();
    compile_benchmark9::test_dispatch_configurations();
    compile_benchmark9::test_filesystem_special();
    compile_benchmark9::test_container_types();
    compile_benchmark9::test_all_encodings();
    compile_benchmark9::test_large_variadic();

    // Namespace 10
    compile_benchmark10::test_basic_types();
    compile_benchmark10::test_numeric_types();
    compile_benchmark10::test_special_types();
    compile_benchmark10::test_multiple_arguments();
    compile_benchmark10::test_markers();
    compile_benchmark10::test_optional_with_defaults();
    compile_benchmark10::test_encoding_arguments();
    compile_benchmark10::test_complex_combinations();
    compile_benchmark10::test_many_arguments();
    compile_benchmark10::test_format_strings();
    compile_benchmark10::test_type_lists();
    compile_benchmark10::test_dispatch_configurations();
    compile_benchmark10::test_filesystem_special();
    compile_benchmark10::test_container_types();
    compile_benchmark10::test_all_encodings();
    compile_benchmark10::test_large_variadic();

    return 0;
}

// NOLINTEND(modernize-use-trailing-return-type)
