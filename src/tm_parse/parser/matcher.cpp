//
// Date       : 01/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/matcher.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse {

Matcher::Matcher() noexcept = default;

Matcher::Matcher(const Parser& parser) noexcept
    : m_Tokens(parser.m_Tokens),
      m_Position(parser.position()) {}

Matcher::Matcher(Parser& parser) noexcept
    : m_Tokens(parser.m_Tokens),
      m_Position(parser.position()) {}

Matcher::Matcher(std::span<const Token> tokens, size_t pos) noexcept
    : m_Tokens(tokens),
      m_Position(pos) {}

bool Matcher::is_eof() const noexcept {
    const Token& tok = peek_real();
    return !tok || tok.is_eof();
}

bool Matcher::try_match(std::span<const tk::TokenKind> kinds) noexcept {
    auto pos = position();

    bool res = std::ranges::all_of(kinds, [this](const auto& kind) -> bool {
        return this->next() == kind;
    });

    if (!res) {
        m_Position = pos;
        return false;
    }

    return true;
}

bool Matcher::try_match_real(std::span<const tk::TokenKind> kinds) noexcept {
    auto pos = position();

    bool res = std::ranges::all_of(kinds, [this](const auto& kind) -> bool {
        return this->next_real() == kind;
    });

    if (!res) {
        m_Position = pos;
        return false;
    }

    return true;
}

const Token& Matcher::peek() const noexcept {
    if (position() >= m_Tokens.size()) {
        return invalid_token_v;
    }
    return m_Tokens[m_Position];
}

const Token& Matcher::peek_real() const noexcept {
    size_t pos = m_Position;

    while (pos < m_Tokens.size()) {
        const Token& tok = m_Tokens[pos++];
        switch (tok.Kind) {
            case tk::LineComment:
            case tk::MultiLineComment:
            case tk::BlankLine:
                continue;
            default:
                return tok;
        }
    }

    return invalid_token_v;
}

Token Matcher::next() noexcept {
    if (position() >= m_Tokens.size()) {
        return Token{};
    }
    return m_Tokens[m_Position++];
}

Token Matcher::next_real() noexcept {
    Token tk{};
    do {
        tk = this->next();

        switch (tk.Kind) {
            case tk::LineComment:
            case tk::MultiLineComment:
            case tk::BlankLine:
                continue;
            default:
                return tk;
        }

    } while (!is_eof());

    return Token{};
}

Token Matcher::maybe(tk::TokenKind kind) noexcept {
    size_t pos = m_Position;
    Token tk = next();

    if (tk == kind) {
        return tk;
    }

    m_Position = pos;
    return Token{tk::InvalidToken};
}

Token Matcher::maybe_real(tk::TokenKind kind) noexcept {
    size_t pos = m_Position;
    Token tk = next_real();

    if (tk == kind) {
        return tk;
    }

    m_Position = pos;
    return Token{tk::InvalidToken};
}

Token Matcher::any(std::span<const tk::TokenKind> kinds) noexcept {
    size_t pos = m_Position;
    Token tk = next();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return Token{tk::InvalidToken};
}

Token Matcher::any_real(std::span<const tk::TokenKind> kinds) noexcept {
    size_t pos = m_Position;
    Token tk = next_real();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return Token{tk::InvalidToken};
}

Token Matcher::not_any(std::span<const tk::TokenKind> kinds) noexcept {
    size_t pos = m_Position;
    Token tk = next();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return Token{tk::InvalidToken};
}

Token Matcher::require(tk::TokenKind kind) {
    size_t pos = m_Position;
    Token tk = next();
    if (tk != kind) {
        m_Position = pos;
        throw TokenError{std::format("expecting token {}", token_type_name(kind)), tk};
    }
    return tk;
}

Token Matcher::require_real(tk::TokenKind kind) {
    size_t pos = m_Position;
    Token tk = next_real();

    if (tk != kind) {
        m_Position = pos;
        throw TokenError{std::format("expecting token {}", token_type_name(kind)), tk};
    }

    return tk;
}

}  // namespace tm_parse
