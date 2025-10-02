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

namespace compile_benchmark4
{
#include "compile_benchmark.inc.cpp"
} // namespace compile_benchmark4

// NOLINTEND(modernize-use-trailing-return-type)
