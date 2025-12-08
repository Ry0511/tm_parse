//
// Date       : 25/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class TupleExpr : public ParserRule {
   private:
    std::vector<std::unique_ptr<ParserRule>> m_Literals;

   public:
    TupleExpr() = default;
    ~TupleExpr() override = default;

   public:
    TupleExpr(const TupleExpr&) = default;
    TupleExpr& operator=(const TupleExpr&) = default;
    TupleExpr(TupleExpr&&) = default;
    TupleExpr& operator=(TupleExpr&&) = default;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(TupleExpr);
};

}  // namespace tm_parse::rules
