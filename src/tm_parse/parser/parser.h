//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/parser/parser_rule.h"

namespace tm_parse {

class Parser {
   private:
    str m_Text;
    Lexer m_Lexer;
    std::vector<Token> m_Tokens;
    size_t m_Position{0};

   public:
    Parser(str text);
    Parser(const fs::path& file);
    ~Parser() = default;

   public:
    bool is_eof() const { return m_Lexer.is_eof(); }

   public:
    std::unique_ptr<ParserRule> parse();

   public:
    str_view text() const noexcept { return m_Text; }
    size_t position() const noexcept { return m_Position; }
    void set_position(size_t pos) noexcept { m_Position = pos; }

   public:
    Token peek(int offset) const noexcept;

   public:
    Token next() noexcept;
    Token next_real() noexcept;
    Token require(tk::TokenKind kind);
    Token require_next_real(tk::TokenKind kind);
    Token maybe(tk::TokenKind kind);
    Token maybe_next_real(tk::TokenKind kind);
};

}  // namespace tm_parse