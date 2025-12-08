//
// Date       : 03/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

// let ID = EXPR
class VariableExpr : public ParserRule {
   private:
    str m_Identifier;
    std::unique_ptr<ParserRule> m_Expr;

   public:
    VariableExpr() noexcept;
    ~VariableExpr() noexcept override;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

    RULE_STATIC_API(VariableExpr);
};

}  // namespace tm_parse::rules