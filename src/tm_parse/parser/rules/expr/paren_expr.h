//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class ParenExpr : public Expr {
   private:
    std::unique_ptr<ParserRule> m_Inner;

   public:
    ParenExpr() = default;
    ~ParenExpr() override;

   public:
    ParenExpr(const ParenExpr&) = delete;
    ParenExpr& operator=(const ParenExpr&) = delete;
    ParenExpr(ParenExpr&&) noexcept;
    ParenExpr& operator=(ParenExpr&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(ParenExpr);
};

}  // namespace tm_parse::rules
