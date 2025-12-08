//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class AssignmentExpr;

class AssignmentExprList : public Expr {
   private:
    std::vector<std::unique_ptr<AssignmentExpr>> m_Assignments;

   public:
    AssignmentExprList() = default;
    ~AssignmentExprList() override;

   public:
    AssignmentExprList(const AssignmentExprList&) = delete;
    AssignmentExprList& operator=(const AssignmentExprList&) = delete;
    AssignmentExprList(AssignmentExprList&&) noexcept;
    AssignmentExprList& operator=(AssignmentExprList&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(AssignmentExprList);
};

}  // namespace tm_parse::rules