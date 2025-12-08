//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/meta_var_expr.h"

#include "tm_parse/parser/rules/common/prop_dot_identifier.h"

namespace tm_parse::rules {

MetaVarExpr::MetaVarExpr() noexcept = default;
MetaVarExpr::~MetaVarExpr() noexcept = default;

MetaVarExpr::MetaVarExpr(MetaVarExpr&&) noexcept = default;
MetaVarExpr& MetaVarExpr::operator=(MetaVarExpr&&) noexcept = default;

bool MetaVarExpr::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::DollarSign) && PropertyDotIdentifier::matches(matcher);
}

std::unique_ptr<MetaVarExpr> MetaVarExpr::create(Parser& parser) {
    Token first = parser.require_real(tk::DollarSign);

    auto rule = std::make_unique<MetaVarExpr>();
    rule->m_Identifier = PropertyDotIdentifier::create(parser);
    rule->m_Identifier->set_parent(*rule);
    rule->post_init(first, rule->m_Identifier->last_token());

    return rule;
}

void MetaVarExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Identifier->visit(func);
}

void MetaVarExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Identifier->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules
