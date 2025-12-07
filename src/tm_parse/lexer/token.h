//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/util/text_region.h"

namespace tm_parse {

struct Token {
    TextRegion Region;
    tk::TokenKind Kind;
    uint16_t Column{std::numeric_limits<uint16_t>::max()};
    uint16_t Line{std::numeric_limits<uint16_t>::max()};
    const str_char* Text{nullptr};

   public:
    constexpr Token()
        : Kind(tk::TokenKind::EndOfInput) {};

    constexpr Token(tk::TokenKind kind)
        : Kind(kind) {};

    constexpr Token(
        tk::TokenKind kind,
        const TextRegion& region,
        int line,
        int column,
        const str_char* text = nullptr
    ) noexcept
        : Kind(kind),
          Region(region),
          Line(line),
          Column(column),
          Text(text) {};

    ~Token() = default;

    constexpr Token(const Token&) = default;
    constexpr Token(Token&&) = default;
    constexpr Token& operator=(const Token&) = default;
    constexpr Token& operator=(Token&&) = default;

   public:
    str_view token_name() const noexcept;
    str to_string() const;
    str_view text() const;
    str_view inner_text() const;
    str literal_text() const;
    bool has_radix() const noexcept;

   public:
    TextRegion extend(const Token& other) const noexcept { return Region.extend(other.Region); }

   public:
    bool is_eof() const noexcept;
    bool is_identifier() const noexcept;
    bool is_keyword() const noexcept;
    bool is_symbol() const noexcept;
    bool is_one_of(const std::span<const tk::TokenKind>& kinds) const noexcept;
    bool is_none_of(const std::span<const tk::TokenKind>& kinds) const noexcept;

   public:
    bool operator==(tk::TokenKind kind) const noexcept;
    bool operator!=(tk::TokenKind kind) const noexcept;
    bool operator==(const Token& other) const noexcept;
    bool operator!=(const Token& other) const noexcept;
    operator bool() const noexcept;
};

constexpr Token invalid_token_v{tk::InvalidToken};

}  // namespace tm_parse