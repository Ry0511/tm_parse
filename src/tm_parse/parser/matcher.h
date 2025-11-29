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

struct MatcherErrorInfo {
    size_t PosIndex{0};             // Position of m_Position in Tokens
    std::span<const Token> Tokens;  // Context tokens relative from m_Position

    const Token& last_valid() const noexcept { return Tokens[PosIndex]; }
    auto front() const noexcept { return Tokens.front(); }
    auto back() const noexcept { return Tokens.back(); }
    auto size() const noexcept { return Tokens.size(); }
    auto empty() const noexcept { return Tokens.empty(); }
};

class Matcher {
   private:
    std::span<const Token> m_Tokens;
    size_t m_Position{0};

   public:
    explicit Matcher() noexcept;
    explicit Matcher(Parser& parser) noexcept;
    explicit Matcher(const Parser& parser) noexcept;
    explicit Matcher(std::span<const Token> parser, size_t pos = 0) noexcept;
    ~Matcher() noexcept = default;

   public:
    const std::span<const Token>& tokens() const noexcept { return m_Tokens; }
    void set_tokens(std::span<const Token> tokens) noexcept { m_Tokens = tokens; }

    size_t position() const noexcept { return m_Position; }
    void set_position(size_t position) noexcept { m_Position = position; }

   public:
    explicit Matcher(const Matcher&) = default;
    Matcher& operator=(const Matcher&) = default;
    explicit Matcher(Matcher&&) = default;
    Matcher& operator=(Matcher&&) = default;

   public:
    bool is_eof() const noexcept;

    template <class T>
    bool matches() noexcept {
        size_t pos = m_Position;
        if (T::matches(*this)) {
            return true;
        }
        m_Position = pos;
        return false;
    }

    template <class... T>
    bool matches_one_of() noexcept {
        static_assert(sizeof...(T) > 0, "atleast 1 type is required");
        return (matches<T>() || ...);
    }

   public:
    bool try_match(std::span<const tk::TokenKind> kinds) noexcept;
    bool try_match_real(std::span<const tk::TokenKind> kinds) noexcept;

   public:
    MatcherErrorInfo get_context_range(size_t max_line_tokens = 8) noexcept;
    str get_error_string(str_view expected) noexcept;

   public:
    const Token& peek() const noexcept;
    const Token& peek_real() const noexcept;
    const Token& next() noexcept;
    const Token& next_real() noexcept;
    const Token& maybe(tk::TokenKind kind) noexcept;
    const Token& maybe_real(tk::TokenKind kind) noexcept;
    const Token& any(const std::span<const tk::TokenKind>& kinds) noexcept;
    const Token& any_real(const std::span<const tk::TokenKind>& kinds) noexcept;
    const Token& not_any(const std::span<const tk::TokenKind>& kinds) noexcept;
    const Token& require(tk::TokenKind kind, const SrcLoc& src = SrcLoc::current());
    const Token& require_real(tk::TokenKind kind, const SrcLoc& src = SrcLoc::current());
};

}  // namespace tm_parse