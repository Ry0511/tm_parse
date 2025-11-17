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

constexpr tk::TokenKind delimiter_tokens[]{
    tk::BlankLine,
    tk::EndOfInput,
    tk::InvalidToken
};

}  // namespace

UnquotedStrLiteral::UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::~UnquotedStrLiteral() noexcept = default;
UnquotedStrLiteral::UnquotedStrLiteral(UnquotedStrLiteral&&) noexcept = default;
UnquotedStrLiteral& UnquotedStrLiteral::operator=(UnquotedStrLiteral&&) noexcept = default;

bool UnquotedStrLiteral::matches(Matcher& matcher) noexcept {
    while (matcher.not_any(delimiter_tokens)) {}
    return true;
}

std::unique_ptr<UnquotedStrLiteral> UnquotedStrLiteral::create(Parser& parser) {
    Token first = parser.next();
    Token last = first;
    Token next = last;

    while (next.is_none_of(delimiter_tokens)) {
        last = next;
        next = parser.next();
    }

    auto rule = std::make_unique<UnquotedStrLiteral>();
    rule->post_init(first, last);
    return rule;
}

}  // namespace tm_parse::rules
