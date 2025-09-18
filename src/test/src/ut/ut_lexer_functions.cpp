//
// Date       : 19/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/lexer/lexer.h"

#undef INFO
#undef WARN

#include "catch.hpp"

namespace tm_parse::tests {

struct TestData {
    str Text;
    Lexer Lexer;
    Token Tk;

    TestData(str text) : Text(text), Lexer(Text), Tk() {};
};

TEST_CASE("assert initial lexer state") {
    TestData t{"First\nSecond\nThird"};
    REQUIRE(!t.Lexer.is_eof());
    REQUIRE(t.Lexer.current_line() == 1);
    REQUIRE(t.Lexer.length() == t.Text.size());
}

TEST_CASE("assert Lexer::require does not throw on valid") {
    TestData t{"First\nSecond\nThird"};

    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));

    // Once EOF is reached all calls should return EOF
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));
}

TEST_CASE("assert Lexer::require does throw on invalid") {
    TestData t{"First\nSecond\nThird"};

    // clang-format off
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier)); REQUIRE_THROWS(t.Lexer.require(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));  REQUIRE_THROWS(t.Lexer.require(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier)); REQUIRE_THROWS(t.Lexer.require(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require(tk::BlankLine));  REQUIRE_THROWS(t.Lexer.require(tk::EndOfInput));
    REQUIRE_NOTHROW(t.Lexer.require(tk::Identifier));
    REQUIRE_NOTHROW(t.Lexer.require(tk::EndOfInput));
    // clang-format on
}

TEST_CASE("assert basic lexer usage") {
    TestData t{"A B C D E F"};

    REQUIRE(!t.Lexer.is_eof());
    REQUIRE(t.Lexer.current_line() == 1);

    do {
        t.Tk = t.Lexer.next_token();
        if (t.Tk != tk::EndOfInput) {
            REQUIRE(t.Tk.is_identifier());
        }
    } while (t.Tk != tk::EndOfInput);

    REQUIRE(t.Lexer.current_line() == 1);
}

}  // namespace tm_parse::tests