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

AssignmentExpr::~AssignmentExpr() = default;
AssignmentExpr::AssignmentExpr(AssignmentExpr&&) noexcept = default;
AssignmentExpr& AssignmentExpr::operator=(AssignmentExpr&&) noexcept = default;

bool AssignmentExpr::matches(Matcher& matcher) noexcept {
    return matches_with_expr_list<assignment_expr_types>(matcher);
}

std::unique_ptr<AssignmentExpr> AssignmentExpr::create(Parser& parser) {
    return create_with_expr_list<assignment_expr_types>(parser);
}

template <class ExprList>
bool AssignmentExpr::matches_with_expr_list(Matcher& matcher) {
    return PropertyAccess::matches(matcher) && matcher.maybe_real(tk::Equal)
           && ExprList{}.matches(matcher);
}

template <class ExprList>
std::unique_ptr<AssignmentExpr> AssignmentExpr::create_with_expr_list(Parser& parser) {
    auto rule = std::make_unique<AssignmentExpr>();

    rule->m_Property = PropertyAccess::create(parser);
    parser.require_real(tk::Equal);
    rule->m_Expr = ExprList{}.create(parser);

    rule->post_init(*rule->m_Property, *rule->m_Expr);
    rule->m_Property->set_parent(*rule);
    rule->m_Expr->set_parent(*rule);

    return rule;
}

void AssignmentExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Property->visit(func);
    m_Expr->visit(func);
}

void AssignmentExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Property->cascade_assign_parents(this);
    m_Expr->cascade_assign_parents(this);
}

void AssignmentExpr::simplify_ast() noexcept {
    m_Property->simplify_ast();
    m_Expr->simplify_ast();
}

template bool AssignmentExpr::matches_with_expr_list<assignment_expr_types>(Matcher&);
template bool AssignmentExpr::matches_with_expr_list<assignment_expr_list_types>(Matcher&);
template std::unique_ptr<AssignmentExpr> AssignmentExpr::create_with_expr_list<assignment_expr_types>(Parser& parser);
template std::unique_ptr<AssignmentExpr> AssignmentExpr::create_with_expr_list<assignment_expr_list_types>(Parser& parser);

}  // namespace tm_parse::rules