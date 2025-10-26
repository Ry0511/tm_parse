//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class Expr : public ParserRule {
   public:
    Expr() = default;
    ~Expr() override = default;

   public:
    Expr(const Expr&) = default;
    Expr& operator=(const Expr&) = default;
    Expr(Expr&&) noexcept = default;
    Expr& operator=(Expr&&) noexcept = default;

   public:
    RULE_STATIC_API(Expr);
};

}  // namespace tm_parse::rules
