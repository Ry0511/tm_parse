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

// TODO: With how this is currently implemented its possible for there to be a single assignment
//  inside an expression list. Since fundamentally A=(B=10) is A being set via an
//  AssignmentExprList. Potentially, this is an ambiguity between a ParenExpr as (1) would be a
//  ParenExpr but (A=1) is not.

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

AssignmentExprList::~AssignmentExprList() = default;
AssignmentExprList::AssignmentExprList(AssignmentExprList&&) noexcept = default;
AssignmentExprList& AssignmentExprList::operator=(AssignmentExprList&&) noexcept = default;

}  // namespace tm_parse::rules