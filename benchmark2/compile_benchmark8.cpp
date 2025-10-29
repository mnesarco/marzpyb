// SPDX-License-Identifier: LGPL-2.1-or-later
// This file is solely for measuring compile time of PyArgParser.hxx
// NOLINTBEGIN(modernize-use-trailing-return-type)

#include "../PyArgParser.hxx"

using namespace pyargs;

namespace parser_benchmark8
{
#include "compile_benchmark.inc.cpp"
}

// NOLINTEND(modernize-use-trailing-return-type)
