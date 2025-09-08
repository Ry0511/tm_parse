//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

namespace tm_parse {

using token_kind_int = uint8_t;

namespace tk {
enum TokenKind : token_kind_int {
    Kw_Set = 0,        // Set
    Kw_None,           // None
    Kw_Level,          // level
    Kw_True,           // True
    Kw_False,          // False
    Kw_Begin,          // Begin
    Kw_Object,         // Object
    Kw_Class,          // Class
    Kw_Name,           // Name
    Kw_Package,        // Package
    Kw_End,            // End
    Kw_Count,          // Count of keywords
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
    Symbols_Count,     // Keep this last
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
constexpr size_t token_type_kw_start = static_cast<size_t>(tk::Kw_Set);
constexpr size_t token_type_kw_end = static_cast<size_t>(tk::Kw_Count);

// Start and end of symbol tokens
constexpr size_t token_type_symbol_start = static_cast<size_t>(tk::LeftParen);
constexpr size_t token_type_symbol_end = static_cast<size_t>(tk::Symbols_Count);

// Count of all tokens, including invalid ones.
constexpr size_t token_type_count = static_cast<size_t>(tk::TokenKind_Count);

////////////////////////////////////////////////////////////////////////////////
// | HELPER FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

constexpr bool is_keyword(token_kind_int kind) noexcept {
    return kind >= token_type_kw_start && kind < token_type_kw_end;
}

constexpr bool is_symbol(token_kind_int kind) noexcept {
    return kind >= token_type_symbol_start && kind < token_type_symbol_end;
}

constexpr bool is_identifier(token_kind_int kind) noexcept {
    return kind == tk::Identifier || is_keyword(kind);
}

}  // namespace tm_parse
