//
// Date       : 15/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"

namespace tm_parse::rules {

namespace {

constexpr tk::TokenKind delimiter_tokens[]{tk::BlankLine, tk::EndOfInput, tk::InvalidToken};

}  // namespace

UnquotedStrLiteral::UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::~UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::UnquotedStrLiteral(UnquotedStrLiteral&&) noexcept = default;
UnquotedStrLiteral& UnquotedStrLiteral::operator=(UnquotedStrLiteral&&) noexcept = default;

bool UnquotedStrLiteral::matches(Matcher& matcher) noexcept {
    Parser* p = matcher.parser();
    if (p != nullptr && !p->parse_state().AllowUnquotedStrings) {
        return false;
    }

    int token_count = 0;
    while (matcher.not_any(delimiter_tokens)) {
        ++token_count;
    }
    return token_count > 0;
}

std::unique_ptr<UnquotedStrLiteral> UnquotedStrLiteral::create(Parser& parser) {
    if (!parser.parse_state().AllowUnquotedStrings) {
        throw std::runtime_error{"can't create UnquotedStrLiteral because it has been disabled"};
    }

    const Token& first = parser.next();
    const Token* last = &first;
    const Token* next = last;
    int count = 0;

    while (next->is_none_of(delimiter_tokens)) {
        last = next;
        next = &parser.next();
        ++count;
    }

    if (count <= 0) {
        throw TokenError{
            parser.get_error_string("atleast 1 token is required in an unquoted string literal"),
            *last
        };
    }

    auto rule = std::make_unique<UnquotedStrLiteral>();
    rule->post_init(first, *last);
    return rule;
}

}  // namespace tm_parse::rules
