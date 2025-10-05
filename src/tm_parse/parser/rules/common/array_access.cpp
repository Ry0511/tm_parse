//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "array_access.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse::rules {

bool ArrayAccess::matches(Matcher& m) noexcept {
    constexpr tk::TokenKind dynamic_access[]{tk::LeftParen, tk::Number, tk::RightParen};
    constexpr tk::TokenKind static_access[]{tk::LeftBracket, tk::Number, tk::RightBracket};
    return m.try_match(dynamic_access) || m.try_match(static_access);
}

std::unique_ptr<ArrayAccess> ArrayAccess::create(Parser& parser) {
    auto rule = std::make_unique<ArrayAccess>();

    Token first{};
    Token index{};
    Token last{};

    if ((first = parser.maybe(tk::LeftParen))) {
        index = parser.require(tk::Number);
        last = parser.require(tk::RightParen);
        rule->m_IsDynamicAccess = true;
    }
    // Static array access
    else {
        first = parser.require(tk::LeftBracket);
        index = parser.require(tk::Number);
        last = parser.require(tk::RightBracket);
        rule->m_IsDynamicAccess = false;
    }

    str_view number_text = index.text();

    try {
        // Number is anything matching: -?[0-9]+(\.[0-9]+)*
        // obviously we won't know if its a valid number until we try to parse it
        // TODO: Replace strtoll with our own utility that takes in str_view or similar
        rule->m_Index = static_cast<int64_t>(std::strtoll(number_text.data(), nullptr, 10));
        rule->m_IsValidNumber = true;
    } catch (const std::out_of_range& err) {
        LOG_TRACE("invalid number in text '{}' with message {}", number_text, err.what());
        rule->m_Index = std::numeric_limits<int64_t>::max();
        rule->m_IsValidNumber = false;
    }

    rule->post_init(first, last);

    return rule;
}

ArrayAccess::~ArrayAccess() = default;

}  // namespace tm_parse::rules