//
// Date       : 15/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

//
// Bit of an annoying rule but effectively its any arbitrary sequence of tokens terminated by an
// EndOfInput or BlankLine token. This rule needs to be used cautiously and can only be used in
// certain contexts. It can't be used in an AssignmentExprList since there is an ambiguity with the
// comma delimited sequences.
//
class UnquotedStrLiteral : public ParserRule {
   public:
    explicit UnquotedStrLiteral() noexcept;
    ~UnquotedStrLiteral() noexcept override;

   public:
    UnquotedStrLiteral(const UnquotedStrLiteral&) noexcept = delete;
    UnquotedStrLiteral& operator=(const UnquotedStrLiteral&) noexcept = delete;
    UnquotedStrLiteral(UnquotedStrLiteral&&) noexcept;
    UnquotedStrLiteral& operator=(UnquotedStrLiteral&&) noexcept;

   public:
    RULE_STATIC_API(UnquotedStrLiteral);
};

}  // namespace tm_parse::rules
