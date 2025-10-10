//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/assignment_expr_list.h"

#include "tm_parse/parser/rules/expr/assignment_expr.h"

namespace tm_parse::rules {

AssignmentExprList::~AssignmentExprList() = default;
AssignmentExprList::AssignmentExprList(AssignmentExprList&&) noexcept = default;
AssignmentExprList& AssignmentExprList::operator=(AssignmentExprList&&) noexcept = default;

bool AssignmentExprList::matches(Matcher& matcher) noexcept {
    if (!matcher.maybe_real(tk::LeftParen)) {
        return false;
    }

    if (!AssignmentExpr::matches(matcher)) {
        return false;
    }

    // Should be of the form:
    //   LeftParen AssignmentExpr ( Comma AssignmentExpr )* RightParen
    while (matcher.maybe_real(tk::Comma)) {
        if (!AssignmentExpr::matches(matcher)) {
            return false;
        }
    }

    return matcher.maybe_real(tk::RightParen);
}

std::unique_ptr<AssignmentExprList> AssignmentExprList::create(Parser& parser) {
    auto rule = std::make_unique<AssignmentExprList>();

    Token first = parser.require_next_real(tk::LeftParen);

    do {
        const auto& expr = rule->m_Assignments.emplace_back(AssignmentExpr::create(parser));
        expr->set_parent(*rule);
    } while (parser.maybe_next_real(tk::Comma));

    Token last = parser.require_next_real(tk::RightParen);
    rule->post_init(first, last);

    return rule;
}

void AssignmentExprList::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    for (const auto& rule : m_Assignments) {
        rule->visit(func);
    }
}

void AssignmentExprList::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    for (const auto& ptr : m_Assignments) {
        ptr->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules