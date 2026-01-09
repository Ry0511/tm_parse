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

namespace {

void validate_result(const Number& val, auto expected, bool expect_int) {
    using T = std::remove_cvref_t<decltype(expected)>;

    if (expect_int) {
        REQUIRE(val.is_integer());
        REQUIRE(!val.is_float());
    } else {
        REQUIRE(!val.is_integer());
        REQUIRE(val.is_float());
    }

    REQUIRE(val.as_int() == static_cast<int64_t>(expected));
    if constexpr (std::is_floating_point_v<T>) {
        // accurate enough for our needs
        REQUIRE(std::abs(val.as_float() - expected) <= std::numeric_limits<double>::epsilon());
    }
}

void check_result_simplified(const str& text, auto value, bool expect_int_result = true) {
    UT_INFO("Testing ~ ( {} ) == {}", text, value);
    Parser parser{text};
    auto expr = ComposedExpr::create(parser);
    expr->simplify_ast();

    auto val = expr->evaluate_numeric_expr();
    REQUIRE(val.has_value());
    if (val.has_value()) {
        validate_result(*val, value, expect_int_result);
    }
}

void check_result_normal(const str& text, auto value, bool expect_int_result = true) {
    UT_INFO("Testing ~ ( {} ) == {}", text, value);
    Parser parser{text};
    auto expr = ComposedExpr::create(parser);

    auto val = expr->evaluate_numeric_expr();
    REQUIRE(val.has_value());
    if (val.has_value()) {
        validate_result(*val, value, expect_int_result);
    }
}

}  // namespace

TEST_CASE("basic int expression not simplified") {
    check_result_normal("1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9", 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
    check_result_normal("1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9", 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9);
    check_result_normal("1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9", 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9);
    check_result_normal("100 / 2 / 2 / 2", 100 / 2 / 2 / 2);
    check_result_normal("(1 + 2) - (3 * 4) / (5 - 6) * (7 / 8)", (1 + 2) - (3 * 4) / (5 - 6) * (7 / 8));
}

TEST_CASE("basic int expression is simplified") {
    check_result_simplified("1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9", 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9);
    check_result_simplified("1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9", 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9);
    check_result_simplified("1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9", 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9);
    check_result_simplified("100 / 2 / 2 / 2", 100 / 2 / 2 / 2);
    check_result_simplified("(1 + 2) - (3 * 4) / (5 - 6) * (7 / 8)", (1 + 2) - (3 * 4) / (5 - 6) * (7 / 8));
}

TEST_CASE("basic float expression not simplified") {
    check_result_normal("1.1 + 2.2 + 3.3 + 4.4 + 5.5", 1.1 + 2.2 + 3.3 + 4.4 + 5.5, false);
    check_result_normal("1.1 - 2.2 - 3.3 - 4.4 - 5.5", 1.1 - 2.2 - 3.3 - 4.4 - 5.5, false);
    check_result_normal("1.1 / 2.2 / 3.3 / 4.4 / 5.5", 1.1 / 2.2 / 3.3 / 4.4 / 5.5, false);
    check_result_normal("1.1 * 2.2 * 3.3 * 4.4 * 5.5", 1.1 * 2.2 * 3.3 * 4.4 * 5.5, false);
    check_result_normal("1.1 * 2.2 / 3.3 - 4.4 + 5.5", 1.1 * 2.2 / 3.3 - 4.4 + 5.5, false);
}

TEST_CASE("basic float expression is simplified") {
    check_result_simplified("1.1 + 2.2 + 3.3 + 4.4 + 5.5", 1.1 + 2.2 + 3.3 + 4.4 + 5.5, false);
    check_result_simplified("1.1 - 2.2 - 3.3 - 4.4 - 5.5", 1.1 - 2.2 - 3.3 - 4.4 - 5.5, false);
    check_result_simplified("1.1 / 2.2 / 3.3 / 4.4 / 5.5", 1.1 / 2.2 / 3.3 / 4.4 / 5.5, false);
    check_result_simplified("1.1 * 2.2 * 3.3 * 4.4 * 5.5", 1.1 * 2.2 * 3.3 * 4.4 * 5.5, false);
    check_result_simplified("1.1 * 2.2 / 3.3 - 4.4 + 5.5", 1.1 * 2.2 / 3.3 - 4.4 + 5.5, false);
}

TEST_CASE("basic mixed expression") {
    // F(Int, Float) => Float
    // F(Float, Int) => Float
    check_result_simplified("1.1 + 2 + 3.3 + 4.4 + 5", 1.1 + 2 + 3.3 + 4.4 + 5, false);
    check_result_simplified("1.1 - 2 - 3.3 - 4.4 - 5", 1.1 - 2 - 3.3 - 4.4 - 5, false);
    check_result_simplified("1.1 / 2 / 3.3 / 4.4 / 5", 1.1 / 2 / 3.3 / 4.4 / 5, false);
    check_result_simplified("1.1 * 2 * 3.3 * 4.4 * 5", 1.1 * 2 * 3.3 * 4.4 * 5, false);
    check_result_simplified("1.1 * 2 / 3.3 - 4.4 + 5", 1.1 * 2 / 3.3 - 4.4 + 5, false);
    check_result_simplified("1.1 * (2 / 3.3 - 4.4 + 5)", 1.1 * (2 / 3.3 - 4.4 + 5), false);
}

}  // namespace tm_parse::tests