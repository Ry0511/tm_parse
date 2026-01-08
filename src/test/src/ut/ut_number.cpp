//
// Date       : 08/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "ut_common.h"
#include "tm_parse/util/number.h"

namespace tm_parse::tests {

TEST_CASE("number default states") {
    REQUIRE(Number{-100}.is_integer());
    REQUIRE(Number{+100}.is_integer());
    REQUIRE(Number{+3.7}.is_float());
    REQUIRE(Number{-3.7}.is_float());
    REQUIRE(Number{-3.7}.as_float() == -3.7);
}

TEST_CASE("number integral numerical operations") {
    auto check_result = [](const Number& res, auto expected) -> void {
        REQUIRE(res.is_integer());
        REQUIRE(!res.is_float());
        REQUIRE(res.as_int() == expected);
    };

    check_result((Number{50} + Number{100}), 150);
    check_result((Number{50} - Number{100}), -50);
    check_result((Number{50} / Number{100}), 0);
    check_result((Number{50} * Number{100}), 5000);

    check_result(-Number{50}, -50);
    check_result(-Number{-50}, 50);
}

TEST_CASE("number float numerical operations") {
    auto check_result = [](const Number& res, auto expected) -> void {
        REQUIRE(!res.is_integer());
        REQUIRE(res.is_float());
        REQUIRE(res.as_int() == expected);
    };

    // Float ? Float
    check_result((Number{50.0} + Number{100.0}), 150);
    check_result((Number{50.0} - Number{100.0}), -50);
    check_result((Number{50.0} / Number{100.0}), 0);
    check_result((Number{50.0} * Number{100.0}), 5000);

    // Float ? Int
    check_result((Number{50.0} + Number{100}), 150);
    check_result((Number{50.0} - Number{100}), -50);
    check_result((Number{50.0} / Number{100}), 0);
    check_result((Number{50.0} * Number{100}), 5000);

    // Int ? Float
    check_result((Number{50} + Number{100.0}), 150);
    check_result((Number{50} - Number{100.0}), -50);
    check_result((Number{50} / Number{100.0}), 0);
    check_result((Number{50} * Number{100.0}), 5000);

    check_result(-Number{50.0}, -50.0);
    check_result(-Number{-50.0}, 50.0);
}

}  // namespace tm_parse::tests