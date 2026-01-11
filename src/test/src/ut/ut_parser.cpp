//
// Date       : 22/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "ut_common.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules.h"

namespace tm_parse::tests {

TEST_CASE("parser rule ::root") {
    Parser parser{TXT("set foo baz (35 * 10 + 100) / A")};
    auto rule = SetCommand::create(parser);

    REQUIRE(rule->root() == nullptr);
    REQUIRE(rule->object_ref().root() == rule.get());
    REQUIRE(rule->object_ref().inner().root() == rule.get());
    REQUIRE(rule->expr().root() == rule.get());
    REQUIRE(rule->expr().as_ref<ComposedExpr>().expr().root() == rule.get());
}

}