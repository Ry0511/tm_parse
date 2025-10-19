//
// Date       : 01/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"

namespace tm_parse {

class Parser;

class Matcher {
   private:
    std::span<const Token> m_Tokens;
    size_t m_Position;

   public:
    explicit Matcher(Parser& parser) noexcept;
    explicit Matcher(const Parser& parser) noexcept;
    explicit Matcher(std::span<const Token> parser, size_t pos = 0) noexcept;
    ~Matcher() noexcept = default;

   public:
    const std::span<const Token>& tokens() const noexcept { return m_Tokens; }
    size_t position() const noexcept { return m_Position; }
    void set_position(size_t position) noexcept { m_Position = position; }

   public:
    explicit Matcher(const Matcher&) = default;
    Matcher& operator=(const Matcher&) = default;
    explicit Matcher(Matcher&&) = default;
    Matcher& operator=(Matcher&&) = default;

   public:
    bool is_eof() const noexcept { return m_Position >= m_Tokens.size(); }

    template <class T>
    bool matches() noexcept {
        size_t pos = m_Position;
        if (T::matches(*this)) {
            return true;
        }
        m_Position = pos;
        return false;
    }

   public:
    bool try_match(std::span<const tk::TokenKind> kinds) noexcept;
    bool try_match_real(std::span<const tk::TokenKind> kinds) noexcept;

   public:
    Token next() noexcept;
    Token next_real() noexcept;
    Token maybe(tk::TokenKind kind) noexcept;
    Token maybe_real(tk::TokenKind kind) noexcept;
    Token any(std::span<const tk::TokenKind> kinds) noexcept;
    Token any_real(std::span<const tk::TokenKind> kinds) noexcept;
    Token require(tk::TokenKind kind);
    Token require_real(tk::TokenKind kind);
};

}  // namespace tm_parse