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

    if (!AssignmentExpr::matches(matcher)) {
        return false;
    }

    // Should be of the form:
    //   LeftParen AssignmentExpr ( Comma AssignmentExpr )* Comma? RightParen
    while (matcher.maybe_real(tk::Comma)) {
        if (matcher.peek_real() == tk::RightParen) {
            matcher.next_real();
            return true;
        }

        if (!matcher.matches<PropertyAccess>()) {
            return false;
        }

        if (!matcher.maybe_real(tk::Equal)) {
            return false;
        }

        if (!assignment_expr_list_types{}.matches(matcher)) {
            return false;
        }
    }

    return matcher.maybe_real(tk::RightParen);
}

std::unique_ptr<AssignmentExprList> AssignmentExprList::create(Parser& parser) {
    auto rule = std::make_unique<AssignmentExprList>();

    Token first = parser.require_real(tk::LeftParen);
    rule->m_Assignments.emplace_back(AssignmentExpr::create(parser));
    rule->m_Assignments.front()->set_parent(*rule);

    while (parser.maybe_real(tk::Comma)) {
        if (parser.peek_real() != tk::RightParen) {
            auto expr = std::make_unique<AssignmentExpr>();

            // Would prefer to use AssignmentExpr::create but we do not allow UnquotedStrLiteral
            // inside expression lists.
            // TODO: This initialisation logic is far from ideal and could be turned into a constructor.
            //  This doesn't just apply here we can apply this to pretty much all rules.
            expr->m_Property = PropertyAccess::create(parser);
            parser.require_real(tk::Equal);
            expr->m_Expr = assignment_expr_list_types{}.create(parser);
            // TODO: as seen manually setting the parent is bug prone
            expr->cascade_assign_parents(rule.get());
            expr->post_init(*expr->m_Property, *expr->m_Expr);

            rule->m_Assignments.emplace_back(std::move(expr));
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