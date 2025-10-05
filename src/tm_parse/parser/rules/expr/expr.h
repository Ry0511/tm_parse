//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

enum class ExprType : int {
    Assignment,      // A = Expr
    AssignmentList,  // A = Expr, B = Expr, C = Expr
    Literal,         // True | False | Number | StringLiteral
};

class Expr : public ParserRule {
   public:
    Expr() = default;
    ~Expr() override = default;

   public:
    Expr(const Expr&) = default;
    Expr& operator=(const Expr&) = default;
    Expr(Expr&&) = default;
    Expr& operator=(Expr&&) = default;

   public:
    RULE_STATIC_API(Expr);
};

}  // namespace tm_parse::rules
