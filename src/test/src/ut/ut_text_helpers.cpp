//
// Date       : 16/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "ut_common.h"

namespace tm_parse::tests {

namespace {

template <class E, class ParseFunc>
void test_parse_number(str text, E expected, ParseFunc func) {
    using T = decltype(func(text))::value_type;
    UT_INFO("{}, {}, {}", text, expected, typeid(T).name());

    std::optional<T> val{std::nullopt};
    REQUIRE_NOTHROW(
        ([&text, &val, &func] { val = func(text); })()
    );
    constexpr bool has_value = !std::is_same_v<E, std::nullptr_t>;

    REQUIRE(val.has_value() == has_value);

    if constexpr (
        has_value
        && (std::is_floating_point_v<E> || std::is_floating_point_v<T>)
    ) {
        // there is a lot more to floating point checks than this but we don't really care for
        // accurate checks here just comparing the integral part would be enough
        auto epsilon = std::numeric_limits<E>::epsilon();
        REQUIRE(std::abs(*val - static_cast<T>(expected)) <= epsilon);

    } else if constexpr (has_value) {
        REQUIRE(*val == static_cast<T>(expected));
    }
}

}  // namespace

TEST_CASE("tm_parse::txt::sanitise_string", "txt") {
    auto test = [] {
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
    using f64_limits = std::numeric_limits<double>;
    test_parse_number(TXT("3.1415"), 3.1415, &txt::parse_double);
    test_parse_number(TXT("-3.1415"), -3.1415, &txt::parse_double);
    test_parse_number(TXT("12345"), 12345, &txt::parse_double);
    test_parse_number(TXT("-12345"), -12345, &txt::parse_double);
    test_parse_number(TXT("0"), 0.0, &txt::parse_double);

    // i32
    using i32_limits = std::numeric_limits<int32_t>;
    test_parse_number(TXT("3.1415"), 3, &txt::parse_int32);
    test_parse_number(TXT("-3.1415"), -3, &txt::parse_int32);
    test_parse_number(TXT("12345"), 12345, &txt::parse_int32);
    test_parse_number(TXT("-12345"), -12345, &txt::parse_int32);
    test_parse_number(TXT("-99999999999999999999999999999999999999"), nullptr, &txt::parse_int32);
    test_parse_number(TXT("99999999999999999999999999999999999999"), nullptr, &txt::parse_int32);
    test_parse_number(std::to_string(i32_limits::max()), i32_limits::max(), &txt::parse_int32);
    test_parse_number(std::to_string(i32_limits::min()), i32_limits::min(), &txt::parse_int32);

    // i64
    using i64_limits = std::numeric_limits<int64_t>;
    test_parse_number(TXT("3.1415"), 3, &txt::parse_int64);
    test_parse_number(TXT("-3.1415"), -3, &txt::parse_int64);
    test_parse_number(TXT("12345"), 12345, &txt::parse_int64);
    test_parse_number(TXT("-12345"), -12345, &txt::parse_int64);
    test_parse_number(TXT("-99999999999999999999999999999999999999"), nullptr, &txt::parse_int64);
    test_parse_number(TXT("99999999999999999999999999999999999999"), nullptr, &txt::parse_int64);
    test_parse_number(std::to_string(i64_limits::max()), i64_limits::max(), &txt::parse_int64);
    test_parse_number(std::to_string(i64_limits::min()), i64_limits::min(), &txt::parse_int64);

    // array index type
    using szt_limits = std::numeric_limits<size_t>;
    test_parse_number(TXT("3.1415"), nullptr, &txt::parse_size_t);
    test_parse_number(TXT("-3.1415"), nullptr, &txt::parse_size_t);
    test_parse_number(TXT("12345"), 12345, &txt::parse_size_t);
    test_parse_number(TXT("-12345"), nullptr, &txt::parse_size_t);
    test_parse_number(TXT("-99999999999999999999999999999999999999"), nullptr, &txt::parse_size_t);
    test_parse_number(TXT("99999999999999999999999999999999999999"), nullptr, &txt::parse_size_t);
    test_parse_number(std::to_string(szt_limits::max()), szt_limits::max(), &txt::parse_size_t);
    test_parse_number(std::to_string(szt_limits::min()), szt_limits::min(), &txt::parse_size_t);
}

}  // namespace tm_parse::tests