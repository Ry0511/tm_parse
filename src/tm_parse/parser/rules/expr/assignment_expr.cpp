//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/property_access.h"
#include "tm_parse/parser/rules/expr/assignment_expr.h"

namespace tm_parse::rules {

bool AssignmentExpr::matches(Matcher& matcher) noexcept {
    return PropertyAccess::matches(matcher) && matcher.maybe_real(tk::Equal)
           && Expr::matches(matcher);
}

std::unique_ptr<AssignmentExpr> AssignmentExpr::create(Parser& parser) {
    auto rule = std::make_unique<AssignmentExpr>();

    rule->m_Property = PropertyAccess::create(parser);
    parser.require_next_real(tk::Equal);
    rule->m_Expr = Expr::create(parser);

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

AssignmentExpr::~AssignmentExpr() = default;
AssignmentExpr::AssignmentExpr(AssignmentExpr&&) = default;
AssignmentExpr& AssignmentExpr::operator=(AssignmentExpr&&) = default;

}  // namespace tm_parse::rules