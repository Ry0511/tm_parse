//
// Date       : 08/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "ut_common.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules.h"

namespace tm_parse::tests {

using namespace tm_parse::rules;

TEST_CASE("basic expression is simplified") {
    Parser parser{TXT("50 + 35")};
    auto expr = ComposedExpr::create(parser);
    expr->simplify_ast();
    auto* lit = expr->expr().is<LiteralExpr>();
    REQUIRE(lit != nullptr);
    REQUIRE(lit->get_opt<Number>()->as_int() == 85);
}

}  // namespace tm_parse::tests