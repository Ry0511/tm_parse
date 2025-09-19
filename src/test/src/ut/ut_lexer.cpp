//
// Date       : 19/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include <utility>

#include "tm_parse/pch.h"
#include "tm_parse/lexer/lexer.h"

#undef INFO
#undef WARN

#include "catch.hpp"

namespace tm_parse::tests {

namespace {

struct TestData {
    str Text;
    Lexer Lexer;
    TestData(str text) : Text(std::move(text)), Lexer(Text) {};
};

}  // namespace

TEST_CASE("assert initial lexer state") {
    TestData t{"First\nSecond\nThird"};
    REQUIRE(!t.Lexer.is_eof());
    REQUIRE(t.Lexer.current_line() == 1);
    REQUIRE(t.Lexer.length() == t.Text.size());
}

TEST_CASE("assert Lexer::next_token usage") {
    TestData t{"First\nSecond\nThird"};

    REQUIRE(t.Lexer.next_token() == tk::Identifier);
    REQUIRE(t.Lexer.next_token() == tk::BlankLine);
    REQUIRE(t.Lexer.next_token() == tk::Identifier);
    REQUIRE(t.Lexer.next_token() == tk::BlankLine);
    REQUIRE(t.Lexer.next_token() == tk::Identifier);
    REQUIRE(t.Lexer.next_token() == tk::EndOfInput);

    REQUIRE(t.Lexer.next_token() == tk::EndOfInput);
    REQUIRE(t.Lexer.next_token() == tk::EndOfInput);
}

TEST_CASE("assert Lexer::peek_token usage") {
    TestData t{"First\nSecond\nThird"};

    auto step_expecting = [](TestData& t, tk::TokenKind kind) -> void {
        const auto& peek = t.Lexer.peek_token();
        const auto& next = t.Lexer.next_token();
        REQUIRE(peek == next);
        REQUIRE(peek == kind);
    };

    step_expecting(t, tk::Identifier);
    step_expecting(t, tk::BlankLine);
    step_expecting(t, tk::Identifier);
    step_expecting(t, tk::BlankLine);
    step_expecting(t, tk::Identifier);

    step_expecting(t, tk::EndOfInput);
    step_expecting(t, tk::EndOfInput);
}

TEST_CASE("assert Lexer::require usage") {
    TestData t{"First\nSecond\nThird"};

    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));

    // Once EOF is reached all calls should return EOF
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));

    // All throwing
    REQUIRE_THROWS(t.Lexer.require(tk::Number));
    REQUIRE_THROWS(t.Lexer.require(tk::StringLiteral));
    REQUIRE_THROWS(t.Lexer.require(tk::Identifier));
}

TEST_CASE("assert Lexer::require_next_real usage") {
    TestData t{R"(
      # Should be skipped
      pi = 1.57075 * 2.0;

    )"};

    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::Equal));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::Number));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::Star));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::Number));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::OtherText));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::EndOfInput));

    // EOF
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require_next_real(tk::EndOfInput));

    // All throwing
    REQUIRE_THROWS(t.Lexer.require_next_real(tk::Number));
    REQUIRE_THROWS(t.Lexer.require_next_real(tk::StringLiteral));
    REQUIRE_THROWS(t.Lexer.require_next_real(tk::Identifier));
}

}  // namespace tm_parse::tests