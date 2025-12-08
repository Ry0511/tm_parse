//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/array_access.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::rules {

namespace {

constexpr tk::TokenKind dynamic_access_seq[]{tk::LeftParen, tk::Number, tk::RightParen};
constexpr tk::TokenKind static_access_seq[]{tk::LeftBracket, tk::Number, tk::RightBracket};

bool has_array_access(Matcher& m) noexcept {
    return m.try_match(dynamic_access_seq) || m.try_match(static_access_seq);
}

void parse_number(ArrayAccessData& data, const Token& token) noexcept {
    auto index = txt::parse_size_t(token.text());
    data.Index = index.has_value() ? index.value() : std::numeric_limits<size_t>::max();
    data.IsValidNumber = index.has_value();
    data.IsInitialised = true;
}

}  // namespace

bool ArrayAccess::matches(Matcher& m) noexcept {
    if (!has_array_access(m)) {
        return false;
    }

    while (has_array_access(m)) {}
    return true;
}

std::unique_ptr<ArrayAccess> ArrayAccess::create(Parser& parser) {
    auto rule = std::make_unique<ArrayAccess>();
    Matcher m = parser.create_matcher();

    Token first = m.next();
    Token last = first;

    if (first != tk::LeftParen && first != tk::LeftBracket) {
        throw TokenError{"expecting ( or [", first};
    }

    size_t i = 0;
    do {

        // dynamic array access
        if (Token open = parser.maybe(tk::LeftParen)) {
            Token index = parser.require(tk::Number);
            last = parser.require(tk::RightParen);
            parse_number(rule->m_Data.at(i), index);
        }
        // static array access
        else if (Token open = parser.require(tk::LeftBracket)) {
            Token index = parser.require(tk::Number);
            last = parser.require(tk::RightBracket);
            parse_number(rule->m_Data.at(i), index);
        }

        m = parser.create_matcher();
        ++i;

    } while (i < max_array_indexes && has_array_access(m));

    rule->post_init(first, last);

    return rule;
}

}  // namespace tm_parse::rules