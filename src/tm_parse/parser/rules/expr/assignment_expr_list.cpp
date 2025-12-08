//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/property_access.h"
#include "tm_parse/parser/rules/util/common_expr.h"

namespace tm_parse::rules {

AssignmentExprList::~AssignmentExprList() = default;
AssignmentExprList::AssignmentExprList(AssignmentExprList&&) noexcept = default;
AssignmentExprList& AssignmentExprList::operator=(AssignmentExprList&&) noexcept = default;

bool AssignmentExprList::matches(Matcher& matcher) noexcept {
    if (!matcher.maybe_real(tk::LeftParen)) {
        return false;
    }

    if (!AssignmentExpr::matches_with_expr_list<assignment_expr_list_types>(matcher)) {
        return false;
    }

    // Should be of the form:
    //   LeftParen AssignmentExpr ( Comma AssignmentExpr )* Comma? RightParen
    while (matcher.maybe_real(tk::Comma)) {
        if (matcher.peek_real() == tk::RightParen) {
            matcher.next_real();
            return true;
        }

        if (!AssignmentExpr::matches_with_expr_list<assignment_expr_list_types>(matcher)) {
            return false;
        }
    }

    return matcher.maybe_real(tk::RightParen);
}

std::unique_ptr<AssignmentExprList> AssignmentExprList::create(Parser& parser) {
    auto rule = std::make_unique<AssignmentExprList>();

    Token first = parser.require_real(tk::LeftParen);
    rule->m_Assignments
        .emplace_back(AssignmentExpr::create_with_expr_list<assignment_expr_list_types>(parser))
        ->set_parent(*rule);

    while (parser.maybe_real(tk::Comma)) {
        if (parser.peek_real() != tk::RightParen) {
            auto& expr = rule->m_Assignments.emplace_back(
                AssignmentExpr::create_with_expr_list<assignment_expr_list_types>(parser)
            );
            expr->set_parent(*rule);
            expr->post_init(*expr->m_Property, *expr->m_Expr);
        }
    }

    Token last = parser.require_real(tk::RightParen);
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