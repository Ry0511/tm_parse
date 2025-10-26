//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/array_access.h"

namespace tm_parse::rules {

ArrayAccess::~ArrayAccess() = default;

namespace {

constexpr tk::TokenKind dynamic_access_seq[]{tk::LeftParen, tk::Number, tk::RightParen};
constexpr tk::TokenKind static_access_seq[]{tk::LeftBracket, tk::Number, tk::RightBracket};

bool has_array_access(Matcher& m) noexcept {
    return m.try_match(dynamic_access_seq) || m.try_match(static_access_seq);
}

void parse_number(ArrayAccessData& data, const Token& token) noexcept {
    const auto number_text = token.text();

    try {
        // Number is anything matching: -?[0-9]+(\.[0-9]+)*
        // obviously we won't know if its a valid number until we try to parse it
        // TODO: Replace strtoll with our own utility that takes in str_view or similar
        data.Index = static_cast<int64_t>(std::strtoll(number_text.data(), nullptr, 10));
        data.IsValidNumber = true;
    } catch (const std::out_of_range& err) {
        LOG_TRACE("invalid number in text '{}' with message {}", number_text, err.what());
        data.Index = std::numeric_limits<int64_t>::max();
        data.IsValidNumber = false;
    }
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
        throw TokenError{"expecting LeftParen or LeftBracket", first};
    }

    do {
        ArrayAccessData data{};

        // dynamic array access
        if (Token open = parser.maybe(tk::LeftParen)) {
            Token index = parser.require(tk::Number);
            last = parser.require(tk::RightParen);
            parse_number(data, index);
        }
        // static array access
        else if (Token open = parser.require(tk::LeftBracket)) {
            Token index = parser.require(tk::Number);
            last = parser.require(tk::RightBracket);
            parse_number(data, index);
        }

        m = parser.create_matcher();

    } while (has_array_access(m));

    rule->post_init(first, last);

    return rule;
}

}  // namespace tm_parse::rules