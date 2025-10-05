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
    std::unique_ptr<Expr> m_Inner;

   public:
    ParenExpr() = default;
    ~ParenExpr() override = default;

   public:
    ParenExpr(const ParenExpr&) = default;
    ParenExpr& operator=(const ParenExpr&) = default;
    ParenExpr(ParenExpr&&) = default;
    ParenExpr& operator=(ParenExpr&&) = default;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

    RULE_STATIC_API(ParenExpr);
};

}  // namespace tm_parse::rules
