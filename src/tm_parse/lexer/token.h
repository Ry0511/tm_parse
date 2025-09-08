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

    constexpr Token() : Kind(tk::TokenKind::EndOfInput) {};
    constexpr Token(tk::TokenKind kind, const TextRegion& region) noexcept : Kind(kind), Region(region) {};

    ~Token() = default;

    constexpr Token(const Token&) = default;
    constexpr Token(Token&&) = default;
    constexpr Token& operator=(const Token&) = default;
    constexpr Token& operator=(Token&&) = default;

   public:
    constexpr bool operator==(tk::TokenKind kind) const noexcept { return Kind == kind; }
    constexpr bool operator!=(tk::TokenKind kind) const noexcept { return Kind != kind; }

    constexpr bool operator==(const Token& other) const noexcept { return Kind == other.Kind; }
    constexpr bool operator!=(const Token& other) const noexcept { return Kind != other.Kind; }
};

}  // namespace tm_parse