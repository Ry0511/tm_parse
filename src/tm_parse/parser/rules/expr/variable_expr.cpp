//
// Date       : 03/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/expr/variable_expr.h"
#include "tm_parse/parser/rules/util/common_expr.h"

namespace tm_parse::rules {

VariableExpr::VariableExpr() noexcept = default;
VariableExpr::~VariableExpr() noexcept = default;

bool VariableExpr::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::Let)
           && matcher.maybe_real(tk::AnyIdentifier)
           && matcher.maybe_real(tk::Equal)
           && variable_expr_types{}.matches(matcher);
}

std::unique_ptr<VariableExpr> VariableExpr::create(Parser& parser) {
    // let ID = EXPR
    const Token& first = parser.require_real(tk::Let);
    const Token& id = parser.require_real(tk::AnyIdentifier);
    parser.require_real(tk::Equal);

    auto rule = std::make_unique<VariableExpr>();
    rule->m_Identifier = str{id.text()};
    rule->m_Expr = variable_expr_types{}.create(parser);
    rule->m_Expr->set_parent(*rule);

    rule->post_init(first, rule->m_Expr->last_token());
    return rule;
}

void VariableExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Expr->visit(func);
}

void VariableExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Expr->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules