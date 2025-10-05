//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class PropertyAccess;

class AssignmentExpr : public Expr {
   private:
    std::unique_ptr<PropertyAccess> m_Property;
    std::unique_ptr<Expr> m_Expr;

   public:
    AssignmentExpr() = default;
    ~AssignmentExpr() override;

   public:
    AssignmentExpr(const AssignmentExpr&) = default;
    AssignmentExpr& operator=(const AssignmentExpr&) = default;
    AssignmentExpr(AssignmentExpr&&) = default;
    AssignmentExpr& operator=(AssignmentExpr&&) = default;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

   public:
    RULE_STATIC_API(AssignmentExpr);
};

}  // namespace tm_parse::rules
