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

   public:  // clang-format off
    Token next() noexcept                       { return m_Lexer.next_token();            }
    Token next_real() noexcept                  { return m_Lexer.next_real_token();       }
    Token require(tk::TokenKind kind)           { return m_Lexer.require(kind);           }
    Token require_next_real(tk::TokenKind kind) { return m_Lexer.require_next_real(kind); }
    Token maybe(tk::TokenKind kind)             { return m_Lexer.maybe(kind);             }
    Token maybe_next_real(tk::TokenKind kind)   { return m_Lexer.maybe_next_real(kind);   }
    // clang-format on
};

}  // namespace tm_parse