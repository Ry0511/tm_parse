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

// TODO: There is a plan to replace str_char* Text with a custom TextSource* or Ref<TextSource> at
//  somepoint so that a few guarantees can be made about the text.

struct Token {
   public:
    tk::TokenKind Kind;
    TextRegion Region;
    uint16_t Column{std::numeric_limits<uint16_t>::max()};
    uint16_t Line{std::numeric_limits<uint16_t>::max()};
    const str_char* Text{nullptr};

   public:
    constexpr Token() : Kind(tk::TokenKind::EndOfInput) {};
    constexpr Token(tk::TokenKind kind) : Kind(kind) {};

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
    constexpr str_view token_name() const noexcept { return tm_parse::token_type_name(Kind); }

   public:
    str to_string() const {
        return std::format(
            "{:>3}:{:<3} - {}, ( {}, {} )",
            Line,
            Column,
            str{token_name()},
            Region.Start,
            Region.End
        );
    }

   public:
    str_view text() const {
        if (Text == nullptr) {
            throw std::runtime_error("token text is unavailable");
        }

        str_view text{Text + Region.Start, Region.length()};
        return text;
    }

    str_view inner_text() const {
        // No inner text, no point in throwing just return the full text, if it exists...
        if (Kind != tk::StringLiteral) {
            return text();
        }

        // Should always be exactly 2
        str_view text = this->text();
        if (text.size() <= 2) {
            return str_view{};
        }

        return text.substr(1, text.size() - 2);
    }

   public:
    bool has_radix() const noexcept {
        if (Text == nullptr || !Region || Kind != tk::Number) {
            return false;
        }
        return this->text().find_first_of(TXT('.')) != str_view::npos;
    }

   public:
    TextRegion extend(const Token& other) const noexcept { return Region.extend(other.Region); }

   public:
    constexpr bool is_eof() const noexcept { return Kind == tk::EndOfInput; }
    constexpr bool is_identifier() const noexcept {
        return tm_parse::is_identifier(Kind) || is_keyword();
    }
    constexpr bool is_keyword() const noexcept { return tm_parse::is_keyword(Kind); }
    constexpr bool is_symbol() const noexcept { return tm_parse::is_symbol(Kind); }

   public:
    constexpr bool is_one_of(const std::span<const tk::TokenKind>& kinds) const noexcept {
        return std::ranges::any_of(kinds, [this](const tk::TokenKind kind) {
            return Kind == kind;
        });
    }

    constexpr bool is_none_of(const std::span<const tk::TokenKind>& kinds) const noexcept {
        return std::ranges::none_of(kinds, [this](const tk::TokenKind kind) {
            return Kind == kind;
        });
    }

   public:
    constexpr bool operator==(tk::TokenKind kind) const noexcept {
        return Kind == kind || (kind == tk::AnyIdentifier && is_identifier());
    }
    constexpr bool operator!=(tk::TokenKind kind) const noexcept { return !operator==(kind); }

    constexpr bool operator==(const Token& other) const noexcept { return operator==(other.Kind); }
    constexpr bool operator!=(const Token& other) const noexcept { return operator!=(other.Kind); }

    constexpr operator bool() const noexcept { return Kind != tk::InvalidToken; }
};

static constexpr Token invalid_token_v{tk::InvalidToken};

}  // namespace tm_parse