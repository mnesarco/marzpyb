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

// Test 1: Basic argument types
void test_basic_types()
{
    constexpr Arguments args1 {arg_int {"x"}};
    constexpr Arguments args2 {arg_float {"y"}};
    constexpr Arguments args3 {arg_double {"z"}};
    constexpr Arguments args4 {arg_bool {"flag"}};
    constexpr Arguments args5 {arg_string {"text"}};
    constexpr Arguments args6 {arg_string_v {"view"}};
    constexpr Arguments args7 {arg_cstr {"cstr"}};
    constexpr Arguments args8 {arg_object {"obj"}};

    // Dummy usage to prevent optimization
    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
    (void)args6;
    (void)args7;
    (void)args8;
}

// Test 2: Numeric types
void test_numeric_types()
{
    constexpr Arguments args1 {arg_uchar {"uc"}};
    constexpr Arguments args2 {arg_short {"s"}};
    constexpr Arguments args3 {arg_ushort {"us"}};
    constexpr Arguments args4 {arg_uint {"ui"}};
    constexpr Arguments args5 {arg_long {"l"}};
    constexpr Arguments args6 {arg_ulong {"ul"}};
    constexpr Arguments args7 {arg_llong {"ll"}};
    constexpr Arguments args8 {arg_ullong {"ull"}};
    constexpr Arguments args9 {arg_ssize {"sz"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
    (void)args6;
    (void)args7;
    (void)args8;
    (void)args9;
}

// Test 3: Special types
void test_special_types()
{
    constexpr Arguments args1 {arg_1byte {"b"}};
    constexpr Arguments args2 {arg_1char {"c"}};
    constexpr Arguments args3 {arg_tuple {"t"}};
    constexpr Arguments args4 {arg_dict {"d"}};
    constexpr Arguments args5 {arg_nnbyte {"nnb"}};
    constexpr Arguments args6 {arg_fspath {"path"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
    (void)args6;
}

// Test 4: Multiple arguments
void test_multiple_arguments()
{
    constexpr Arguments args1 {arg_int {"x"}, arg_float {"y"}};
    constexpr Arguments args2 {arg_int {"x"}, arg_float {"y"}, arg_string {"name"}};
    constexpr Arguments args3 {
        arg_int {"x"}, arg_float {"y"}, arg_string_v {"name"}, arg_bool {"flag"}};
    constexpr Arguments args4 {
        arg_int {"a"}, arg_float {"b"}, arg_double {"c"}, arg_string {"d"}, arg_bool {"e"}};
    constexpr Arguments args5 {arg_int {"a"},
                               arg_float {"b"},
                               arg_double {"c"},
                               arg_string {"d"},
                               arg_bool {"e"},
                               arg_object {"f"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
}

// Test 5: Markers
void test_markers()
{
    constexpr Arguments args1 {arg_pos_only {}, arg_int {"x"}};
    constexpr Arguments args2 {arg_int {"x"}, arg_kw_only {}, arg_float {"y"}};
    constexpr Arguments args3 {arg_int {"x"}, arg_optionals {}, arg_float {"y"}};
    constexpr Arguments args4 {
        arg_int {"a"}, arg_pos_only {}, arg_float {"b"}, arg_kw_only {}, arg_string {"c"}};
    constexpr Arguments args5 {
        arg_int {"a"}, arg_float {"b"}, arg_optionals {}, arg_string {"c"}, arg_bool {"d"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
}

// Test 6: Optional arguments with defaults
void test_optional_with_defaults()
{
    constexpr Arguments args1 {
        arg_int {"x"},
         arg_optionals {},
         arg_float {"y", 5.0f}
    };
    constexpr Arguments args2 {
        arg_int {"x"},
         arg_optionals {},
         arg_float {"y", 1.0f},
         arg_string {"name"}
    };
    constexpr Arguments args3 {
        arg_int {"x"}, arg_float {"y"}, arg_optionals {}, arg_bool {"flag"}, arg_string {"text"}};

    (void)args1;
    (void)args2;
    (void)args3;
}

// Test 7: Encoding arguments
void test_encoding_arguments()
{
    constexpr Arguments args1 {arg_enc_cstr<enc_utf8> {"utf8_text"}};
    constexpr Arguments args2 {arg_enc_cstr<enc_utf16> {"utf16_text"}};
    constexpr Arguments args3 {arg_enc_cstr<enc_ascii> {"ascii_text"}};
    constexpr Arguments args4 {arg_enc_cstr<enc_latin1> {"latin1_text"}};
    constexpr Arguments args5 {arg_int {"x"}, arg_enc_cstr<enc_utf8> {"text"}, arg_float {"y"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
}

// Test 8: Complex combinations
void test_complex_combinations()
{
    constexpr Arguments args1 {arg_int {"x"},
                               arg_float {"y"},
                               arg_string {"name"},
                               arg_bool {"flag"},
                               arg_double {"precision"}};

    constexpr Arguments args2 {
        arg_int {"x"},
        arg_optionals {},
        arg_float {"y", 5.0f},
        arg_object {"target"},
        arg_bool {"flag"},
        arg_string_v {"name"},
        arg_enc_cstr<enc_utf8> {"ux"},
        arg_fspath {"path"}
    };

    constexpr Arguments args3 {arg_int {"a"},
                               arg_pos_only {},
                               arg_float {"b"},
                               arg_string {"c"},
                               arg_kw_only {},
                               arg_double {"d"},
                               arg_bool {"e"}};

    constexpr Arguments args4 {
        arg_int {"x"},
        arg_float {"y"},
        arg_optionals {},
        arg_string {"name"},
        arg_bool {"flag"},
        arg_double {"precision", 0.001}
    };

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
}

// Test 9: Many arguments (stress test)
void test_many_arguments()
{
    constexpr Arguments args {arg_int {"a1"},
                              arg_int {"a2"},
                              arg_int {"a3"},
                              arg_int {"a4"},
                              arg_int {"a5"},
                              arg_float {"f1"},
                              arg_float {"f2"},
                              arg_float {"f3"},
                              arg_double {"d1"},
                              arg_double {"d2"},
                              arg_string {"s1"},
                              arg_string {"s2"},
                              arg_string {"s3"},
                              arg_bool {"b1"},
                              arg_bool {"b2"},
                              arg_object {"o1"},
                              arg_object {"o2"}};

    (void)args;
}

// Test 10: Format string generation
void test_format_strings()
{
    // Test various format string combinations
    constexpr auto fmt1 = FmtString {"hello"} + FmtString {" world"};
    constexpr auto fmt2 = FmtString {"a"} + FmtString {"b"} + FmtString {"c"};
    constexpr auto fmt3 = FmtString {"x"} + FmtString {"y"} + FmtString {"z"} + FmtString {"w"};

    // Test format string generation from arguments
    constexpr Arguments args1 {arg_int {"x"}, arg_float {"y"}};
    constexpr Arguments args2 {arg_int {"x"}, arg_float {"y"}, arg_string_v {"name"}};
    constexpr Arguments args3 {arg_int {"x"}, arg_optionals {}, arg_float {"y"}};

    (void)fmt1;
    (void)fmt2;
    (void)fmt3;
    (void)args1;
    (void)args2;
    (void)args3;
}

// Test 11: Type list operations
void test_type_lists()
{
    using list1 = detail::type_list<int, float>;
    using list2 = detail::type_list<double, char>;
    using list3 = detail::type_list<int, float, double, char>;
    using list4 = detail::type_list<>;

    // Type list concatenation
    using concat1 = decltype(detail::concat_types(std::declval<list1>(), std::declval<list2>()));
    using concat2 = decltype(detail::concat_types(std::declval<list1>(), std::declval<list4>()));

    // Type list to tuple conversion
    using tuple1 = typename detail::type_list_to_tuple<list1>::type;
    using tuple2 = typename detail::type_list_to_tuple<list3>::type;

    (void)sizeof(concat1);
    (void)sizeof(concat2);
    (void)sizeof(tuple1);
    (void)sizeof(tuple2);
}

// Test 12: Multiple dispatch configurations
void test_dispatch_configurations()
{
    constexpr Arguments config1 {arg_int {"x"}};
    constexpr Arguments config2 {arg_float {"y"}};
    constexpr Arguments config3 {arg_string {"text"}};
    constexpr Arguments config4 {arg_int {"x"}, arg_float {"y"}};
    constexpr Arguments config5 {arg_string {"text"}, arg_bool {"flag"}};
    constexpr Arguments config6 {arg_int {"x"}, arg_float {"y"}, arg_string {"z"}};
    constexpr Arguments config7 {
        arg_int {"a"}, arg_float {"b"}, arg_double {"c"}, arg_string {"d"}};

    (void)config1;
    (void)config2;
    (void)config3;
    (void)config4;
    (void)config5;
    (void)config6;
    (void)config7;
}

// Test 13: Filesystem and special types
void test_filesystem_special()
{
    constexpr Arguments args1 {arg_fspath {"path1"}};
    constexpr Arguments args2 {arg_fspath {"path1"}, arg_fspath {"path2"}};
    constexpr Arguments args3 {arg_int {"x"}, arg_fspath {"path"}, arg_float {"y"}};
    constexpr Arguments args4 {arg_string {"name"}, arg_fspath {"path"}, arg_bool {"flag"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
}

// Test 14: Tuple and dict arguments
void test_container_types()
{
    constexpr Arguments args1 {arg_tuple {"t"}};
    constexpr Arguments args2 {arg_dict {"d"}};
    constexpr Arguments args3 {arg_tuple {"t"}, arg_dict {"d"}};
    constexpr Arguments args4 {arg_int {"x"}, arg_tuple {"t"}, arg_dict {"d"}, arg_float {"y"}};

    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
}

// Test 15: All encoding types
void test_all_encodings()
{
    constexpr Arguments utf8_args {arg_enc_cstr<enc_utf8> {"text"}};
    constexpr Arguments utf16_args {arg_enc_cstr<enc_utf16> {"text"}};
    constexpr Arguments ascii_args {arg_enc_cstr<enc_ascii> {"text"}};
    constexpr Arguments latin1_args {arg_enc_cstr<enc_latin1> {"text"}};

    constexpr Arguments mixed_args {arg_enc_cstr<enc_utf8> {"utf8"},
                                    arg_enc_cstr<enc_utf16> {"utf16"},
                                    arg_enc_cstr<enc_ascii> {"ascii"},
                                    arg_enc_cstr<enc_latin1> {"latin1"}};

    (void)utf8_args;
    (void)utf16_args;
    (void)ascii_args;
    (void)latin1_args;
    (void)mixed_args;
}

// Test 16: Large variadic template instantiation
void test_large_variadic()
{
    constexpr Arguments mega_args {
        arg_int {"i1"},    arg_int {"i2"},    arg_int {"i3"},    arg_int {"i4"},
        arg_float {"f1"},  arg_float {"f2"},  arg_float {"f3"},  arg_float {"f4"},
        arg_double {"d1"}, arg_double {"d2"}, arg_double {"d3"}, arg_double {"d4"},
        arg_string {"s1"}, arg_string {"s2"}, arg_string {"s3"}, arg_string {"s4"},
        arg_bool {"b1"},   arg_bool {"b2"},   arg_bool {"b3"},   arg_bool {"b4"},
        arg_long {"l1"},   arg_long {"l2"},   arg_long {"l3"},   arg_long {"l4"},
        arg_short {"sh1"}, arg_short {"sh2"}, arg_uchar {"uc1"}, arg_uchar {"uc2"},
        arg_llong {"ll1"}, arg_llong {"ll2"}, arg_object {"o1"}, arg_object {"o2"}};

    (void)mega_args;
}

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
