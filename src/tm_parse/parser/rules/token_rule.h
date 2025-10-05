//
// Date       : 24/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "parser_rule.h"

namespace tm_parse {

// Represent any token from the parser, catch all rule
class TokenRule : public ParserRule {
   private:
    Token m_Token;

   public:
    TokenRule() = default;
    ~TokenRule() override = default;

   public:
    RULE_STATIC_API(TokenRule);
};

}  // namespace tm_parse
