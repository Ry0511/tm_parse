//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "rules/parser_rule.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/parser/matcher.h"

namespace tm_parse {

class Parser : public Matcher {
   private:
    str m_Text;
    std::vector<Token> m_Tokens;

   public:
    explicit Parser(str text);
    explicit Parser(const fs::path& file);
    ~Parser() = default;

   public:
    std::unique_ptr<ParserRule> parse();

   public:
    str_view text() const noexcept { return m_Text; }

   public:
    Matcher create_matcher() const noexcept { return Matcher{*this}; }

   private:
    friend class Matcher;
};

}  // namespace tm_parse