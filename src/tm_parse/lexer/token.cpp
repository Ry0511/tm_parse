//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse {

str_view Token::token_name() const noexcept {
    return tm_parse::token_type_name(Kind);
}

str Token::to_string() const {
    return std::format("{:>3}:{:<3} {}", Line, Column, token_name());
}

str_view Token::text() const {
    if (Text == nullptr) {
        throw std::runtime_error("token text is unavailable");
    }

    str_view text{Text + Region.Start, Region.length()};
    return text;
}

str_view Token::inner_text() const {
    if (Kind != tk::StringLiteral) {
        return text();
    }

    str_view text = this->text();
    if (text.size() <= 2) {
        return str_view{};
    }

    return text.substr(1, text.size() - 2);
}

str Token::literal_text() const {
    if (Kind != tk::StringLiteral) {
        return str{text()};
    }
    return txt::sanitise_string(inner_text());
}

bool Token::has_radix() const noexcept {
    if (Text == nullptr || !Region || Kind != tk::Number) {
        return false;
    }
    return this->text().find_first_of(TXT('.')) != str_view::npos;
}

bool Token::is_eof() const noexcept {
    return Kind == tk::EndOfInput;
}

bool Token::is_identifier() const noexcept {
    return tm_parse::is_identifier(Kind) || is_keyword();
}

bool Token::is_keyword() const noexcept {
    return tm_parse::is_keyword(Kind);
}

bool Token::is_symbol() const noexcept {
    return tm_parse::is_symbol(Kind);
}

bool Token::is_one_of(const std::span<const tk::TokenKind>& kinds) const noexcept {
    return std::ranges::any_of(kinds, [this](const tk::TokenKind kind) {
        return Kind == kind;
    });
}

bool Token::is_none_of(const std::span<const tk::TokenKind>& kinds) const noexcept {
    return std::ranges::none_of(kinds, [this](const tk::TokenKind kind) {
        return Kind == kind;
    });
}

bool Token::operator==(tk::TokenKind kind) const noexcept {
    return Kind == kind || (kind == tk::AnyIdentifier && is_identifier());
}

bool Token::operator!=(tk::TokenKind kind) const noexcept {
    return !operator==(kind);
}

bool Token::operator==(const Token& other) const noexcept {
    return operator==(other.Kind);
}

bool Token::operator!=(const Token& other) const noexcept {
    return operator!=(other.Kind);
}

Token::operator bool() const noexcept {
    return Kind != tk::InvalidToken;
}

}  // namespace tm_parse
