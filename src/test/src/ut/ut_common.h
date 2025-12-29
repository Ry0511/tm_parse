//
// Date       : 16/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "../catch2/catch.hpp"

// defer to catch::$INFO
#define UT_INFO(...) INFO(std::format(__VA_ARGS__))

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/lexer/token_error.h"
#include "tm_parse/util/text_helpers.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/parser_rule.h"
#include "tm_parse/parser/rules/primary/program.h"

namespace tm_parse::tests {

using namespace tm_parse;
using namespace tm_parse::rules;

}  // namespace tm_parse::tests