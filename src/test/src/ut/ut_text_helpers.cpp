//
// Date       : 16/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "ut_common.h"

namespace tm_parse::tests {

TEST_CASE("tm_parse::txt::sanitise_string", "txt") {

    auto test = []{
        REQUIRE(txt::sanitise_string(R"(\tHello\nThere)") == "\tHello\nThere");
        REQUIRE(txt::sanitise_string(R"()").empty());
        REQUIRE(txt::sanitise_string(R"(\n)") == "\n");
        REQUIRE(txt::sanitise_string(R"(\\)") == "\\");
        REQUIRE(txt::sanitise_string(R"(\t)") == "\t");
        REQUIRE(txt::sanitise_string(R"(\t\n\t\n\\)") == "\t\n\t\n\\");
        REQUIRE(txt::sanitise_string(R"(12345)") == "12345");
        REQUIRE(txt::sanitise_string(R"(\t12345\n\t54321)") == "\t12345\n\t54321");
    };

    REQUIRE_NOTHROW(test());
}

TEST_CASE("tm_parse::txt::") {

    // f64
    REQUIRE(txt::parse_double("3.1415").has_value());
    REQUIRE(txt::parse_double("-3.1415").has_value());
    REQUIRE(txt::parse_double("12345").has_value());
    REQUIRE(txt::parse_double("-12345").has_value());
    REQUIRE(txt::parse_double("-99999999999999999999999999999999999999").has_value());
    REQUIRE(txt::parse_double("99999999999999999999999999999999999999").has_value());
    REQUIRE(txt::parse_double(std::to_string(std::numeric_limits<double>::max())).has_value());
    REQUIRE(txt::parse_double(std::to_string(std::numeric_limits<double>::min())).has_value());

    // i32
    REQUIRE(txt::parse_int32("3.1415").has_value());
    REQUIRE(txt::parse_int32("-3.1415").has_value());
    REQUIRE(txt::parse_int32("12345").has_value());
    REQUIRE(txt::parse_int32("-12345").has_value());
    REQUIRE(!txt::parse_int32("-99999999999999999999999999999999999999").has_value());
    REQUIRE(!txt::parse_int32("99999999999999999999999999999999999999").has_value());
    REQUIRE(txt::parse_int32(std::to_string(std::numeric_limits<int32_t>::max())).has_value());

    // i64
    REQUIRE(txt::parse_int64("3.1415").has_value());
    REQUIRE(txt::parse_int64("-3.1415").has_value());
    REQUIRE(txt::parse_int64("12345").has_value());
    REQUIRE(txt::parse_int64("-12345").has_value());
    REQUIRE(!txt::parse_int64("-99999999999999999999999999999999999999").has_value());
    REQUIRE(!txt::parse_int64("99999999999999999999999999999999999999").has_value());
    REQUIRE(txt::parse_int64(std::to_string(std::numeric_limits<int64_t>::max())).has_value());

    // array index type
    REQUIRE(!txt::parse_size_t("3.1415").has_value());
    REQUIRE(!txt::parse_size_t("-3.1415").has_value());
    REQUIRE(!txt::parse_size_t("-12345").has_value());
    REQUIRE(!txt::parse_size_t("-99999999999999999999999999999999999999").has_value());
    REQUIRE(!txt::parse_size_t("99999999999999999999999999999999999999").has_value());
    REQUIRE(txt::parse_size_t("0").has_value());
    REQUIRE(txt::parse_size_t("12345").has_value());
    REQUIRE(txt::parse_size_t(std::to_string(std::numeric_limits<size_t>::max())).has_value());
}

}