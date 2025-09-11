//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include <algorithm>

#include "tm_parse/pch.h"

namespace tm_parse {

using token_kind_int = uint8_t;

namespace tk {
enum TokenKind : token_kind_int {
    Set = 0,           // Set
    None,              // None
    Level,             // level
    True,              // True
    False,             // False
    Begin,             // Begin
    Object,            // Object
    Class,             // Class
    Name,              // Name
    Package,           // Package
    End,               // End
    Keyword_Count,     // Count of keywords
    LeftParen,         // (
    RightParen,        // )
    Dot,               // .
    Colon,             // :
    Slash,             // /
    Star,              // *
    Comma,             // ,
    LeftBracket,       // [
    RightBracket,      // ]
    Equal,             // =
    SingleQuote,       // '
    QuestionMark,      // ?
    DollarSign,        // $
    LeftBrace,         // {
    RightBrace,        // }
    Symbol_Count,      // Keep this last
    Number,            // [0-9]+ ( \. [0-9]+ )?
    Identifier,        // [a-zA-Z_][\w\d_]+
    StringLiteral,     // ".*?"
    NameLiteral,       // '.*?'
    OtherText,         // Any unhandled/unknown text literal
    LineComment,       // # ...
    MultiLineComment,  // /* ... */
    BlankLine,         // [\n\r]
    EndOfInput,        // EOF
    TokenKind_Count,   // Keep this last
};
}

////////////////////////////////////////////////////////////////////////////////
// | HELPER GLOBALS |
////////////////////////////////////////////////////////////////////////////////

// Start and End of keyword tokens
constexpr size_t token_type_kw_start = static_cast<size_t>(tk::Set);
constexpr size_t token_type_kw_end = static_cast<size_t>(tk::Keyword_Count);

// Start and end of symbol tokens
constexpr size_t token_type_symbol_start = static_cast<size_t>(tk::LeftParen);
constexpr size_t token_type_symbol_end = static_cast<size_t>(tk::Symbol_Count);

// Count of all tokens, including invalid ones.
constexpr size_t token_type_count = static_cast<size_t>(tk::TokenKind_Count);

constexpr std::array<std::string_view, token_type_count + 1> token_type_names{
    "Set",
    "None",
    "Level",
    "True",
    "False",
    "Begin",
    "Object",
    "Class",
    "Name",
    "Package",
    "End",
    "Keyword_Count",

    "LeftParen",
    "RightParen",
    "Dot",
    "Colon",
    "Slash",
    "Star",
    "Comma",
    "LeftBracket",
    "RightBracket",
    "Equal",
    "SingleQuote",
    "QuestionMark",
    "DollarSign",
    "LeftBrace",
    "RightBrace",
    "Symbol_Count",

    "Number",
    "Identifier",
    "StringLiteral",
    "NameLiteral",
    "OtherText",
    "LineComment",
    "MultiLineComment",
    "BlankLine",
    "EndOfInput",
    "TokenKind_Count",
};

constexpr std::string_view token_type_name(tk::TokenKind kind) noexcept {
    return token_type_names.at(static_cast<size_t>(kind));
}

consteval size_t largest_keyword_length() {
    size_t largest = 0;
    for (size_t i = token_type_kw_start; i < token_type_kw_end; ++i) {
        largest = std::max(token_type_names.at(i).size(), largest);
    }
    return largest;
}

consteval size_t smallest_keyword_length() {
    size_t smallest = std::numeric_limits<size_t>::max();
    for (size_t i = 0; i < token_type_count; ++i) {
        smallest = std::min(token_type_names.at(i).size(), smallest);
    }
    return smallest;
}

////////////////////////////////////////////////////////////////////////////////
// | HELPER FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

constexpr bool is_keyword(tk::TokenKind kind) noexcept {
    return kind >= token_type_kw_start && kind < token_type_kw_end;
}

constexpr bool is_symbol(tk::TokenKind kind) noexcept {
    return kind >= token_type_symbol_start && kind < token_type_symbol_end;
}

constexpr bool is_identifier(tk::TokenKind kind) noexcept {
    return kind == tk::Identifier || is_keyword(kind);
}

}  // namespace tm_parse
