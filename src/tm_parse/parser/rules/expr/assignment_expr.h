//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"
#include "tm_parse/parser/rules/common/property_access.h"

namespace tm_parse::rules {

class AssignmentExpr : public Expr {
   private:
    friend class AssignmentExprList;
    std::unique_ptr<PropertyAccess> m_Property{nullptr};
    std::unique_ptr<ParserRule> m_Expr{nullptr};

   public:
    AssignmentExpr() = default;
    ~AssignmentExpr() override;

   public:
    AssignmentExpr(const AssignmentExpr&) = delete;
    AssignmentExpr& operator=(const AssignmentExpr&) = delete;
    AssignmentExpr(AssignmentExpr&&) noexcept;
    AssignmentExpr& operator=(AssignmentExpr&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    const ParserRule& property() const { return m_Property->property(); }
    const ParserRule& expr() const { return *m_Expr; }

   public:
    RULE_STATIC_API(AssignmentExpr);

    template <class ExprList>
    static bool matches_with_expr_list(Matcher&);

    template <class ExprList>
    static std::unique_ptr<AssignmentExpr> create_with_expr_list(Parser&);
};

}  // namespace tm_parse::rules
