//
// Date       : 06/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"
#include "tm_parse/parser/rules/expr/identifier_ref_expr.h"

namespace tm_parse::rules {

// TODO: need to look into this rule again

IdentifierRefExpr::IdentifierRefExpr() = default;
IdentifierRefExpr::~IdentifierRefExpr() = default;
IdentifierRefExpr::IdentifierRefExpr(IdentifierRefExpr&&) noexcept = default;
IdentifierRefExpr& IdentifierRefExpr::operator=(IdentifierRefExpr&&) noexcept = default;

bool IdentifierRefExpr::matches(Matcher& matcher) noexcept {
    return PropertyDotIdentifier::matches(matcher);
}

std::unique_ptr<IdentifierRefExpr> IdentifierRefExpr::create(Parser& parser) {
    auto rule = std::make_unique<IdentifierRefExpr>();
    rule->m_Identifier = PropertyDotIdentifier::create(parser);
    rule->copy_state(*rule->m_Identifier);
    return rule;
}

void IdentifierRefExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Identifier->visit(func);
}

void IdentifierRefExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Identifier->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules