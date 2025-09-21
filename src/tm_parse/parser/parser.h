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
    str_view text() const noexcept { return m_Text; }
};

}  // namespace tm_parse