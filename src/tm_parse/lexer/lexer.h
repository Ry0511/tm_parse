//
// Date       : 27/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"

namespace tm_parse {

class Lexer {
   public:
    Lexer(str_view source);
    ~Lexer() = default;

    Lexer(const Lexer&) = default;
    Lexer(Lexer&&) = default;
    Lexer& operator=(const Lexer&) = default;
    Lexer& operator=(Lexer&&) = default;

   public:
    uint32_t current_line() const noexcept { return m_Line; }
    size_t length() const noexcept { return m_Text.size(); }
    bool is_eof() const noexcept { return m_Pos >= m_Text.size(); }

   public:
    Token next_token();
    Token next_real_token();
    Token peek_token();

   public:
    Token require(tk::TokenKind kind);
    Token require_next_real(tk::TokenKind kind);

   private:
    Lexer save_state() const noexcept { return *this; }
    void restore_state(const Lexer& lexer) noexcept { *this = lexer; }

    void skip_whitespace();
    str_char peek() const;
    str_char peek(int offset) const;
    str_char advance();

   private:
    Token create_token(tk::TokenKind kind);
    Token next_token_impl();

   private:
    Token read_identifier();
    Token read_number();
    Token read_other();
    Token read_line_comment();
    Token read_multiline_comment();
    Token read_string_literal();

   private:
    str_view m_Text;    // The text buffer to tokenise
    size_t m_Start{0};  // Current start of token, if any
    size_t m_Pos{0};    // Current stream index
    int m_Line{1};      // 1 Based
    int m_Column{1};    // 1 Based
};

}  // namespace tm_parse
