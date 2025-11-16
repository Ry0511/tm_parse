//
// Date       : 15/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include <algorithm>

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"

namespace tm_parse::rules {

namespace {

// TODO: This hasn't been tested nor validated yet

// Not sure if tk::LineComment and tk::MultiLineComment should be delimiters...
constexpr tk::TokenKind delimiter_tokens[]{
    tk::BlankLine,   // new lines must be provided as \n
    tk::EndOfInput,  // no brainer here
    tk::InvalidToken
};

bool is_delimiter_token(const Token& tok) noexcept {
    return std::ranges::any_of(delimiter_tokens, [&tok](const auto& kind) -> bool {
        return tok.Kind == kind;
    });
}

}  // namespace

UnquotedStrLiteral::UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::~UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::UnquotedStrLiteral(UnquotedStrLiteral&&) noexcept = default;
UnquotedStrLiteral& UnquotedStrLiteral::operator=(UnquotedStrLiteral&&) noexcept = default;

bool UnquotedStrLiteral::matches(Matcher& matcher) noexcept {
    // Consume everything that is not a delimiter token
    while (!matcher.any(delimiter_tokens)) {}
    return true;
}

std::unique_ptr<UnquotedStrLiteral> UnquotedStrLiteral::create(Parser& parser) {
    Token first = parser.next();
    Token last = first;

    bool terminated = is_delimiter_token(last);
    while (Token tok = parser.next()) {
        terminated = is_delimiter_token(tok);
        if (!terminated) {
            last = tok;
        }
    }

    auto rule = std::make_unique<UnquotedStrLiteral>();
    rule->post_init(first, last);
    return rule;
}

}  // namespace tm_parse::rules
