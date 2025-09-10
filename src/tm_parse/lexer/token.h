//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_type.h"
#include "tm_parse/util/text_region.h"

namespace tm_parse {

struct Token {
   public:
    tk::TokenKind Kind;
    TextRegion Region;
    int Line{-1};
    int Column{-1};

   public:
    constexpr Token() : Kind(tk::TokenKind::EndOfInput) {};

    constexpr Token(tk::TokenKind kind, const TextRegion& region, int line, int column) noexcept
        : Kind(kind),
          Region(region),
          Line(line),
          Column(column) {};

    ~Token() = default;

    constexpr Token(const Token&) = default;
    constexpr Token(Token&&) = default;
    constexpr Token& operator=(const Token&) = default;
    constexpr Token& operator=(Token&&) = default;

   public:
    constexpr str_view token_name() const noexcept { return tm_parse::token_type_name(Kind); }

   public:
    constexpr bool is_eof() const noexcept { return Kind == tk::EndOfInput; }
    constexpr bool is_identifier() const noexcept { return tm_parse::is_identifier(Kind); }
    constexpr bool is_keyword() const noexcept { return tm_parse::is_keyword(Kind); }
    constexpr bool is_symbol() const noexcept { return tm_parse::is_symbol(Kind); }

   public:
    constexpr bool operator==(tk::TokenKind kind) const noexcept { return Kind == kind; }
    constexpr bool operator!=(tk::TokenKind kind) const noexcept { return Kind != kind; }

    constexpr bool operator==(const Token& other) const noexcept { return Kind == other.Kind; }
    constexpr bool operator!=(const Token& other) const noexcept { return Kind != other.Kind; }
};

}  // namespace tm_parse