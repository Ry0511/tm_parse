//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/paren_expr.h"
#include "tm_parse/parser/rules/util/common_expr.h"

namespace tm_parse::rules {

// TODO: Think this rule can be removed as it does not have any real usages. It also conflicts with
//  ComposedExpr which creates this ambiguity when both are present:
//    > set foo baz (A + B) * 100
//  This could parse as a SetCommand(ParenExpr(ComposedExpr["A + B"])) instead of SetCommand(ComposedExpr["(A + B) * 100"])
//

ParenExpr::~ParenExpr() = default;
ParenExpr::ParenExpr(ParenExpr&&) noexcept = default;
ParenExpr& ParenExpr::operator=(ParenExpr&&) noexcept = default;

bool ParenExpr::matches(Matcher& matcher) noexcept {
    if (matcher.maybe_real(tk::LeftParen)) {
        return paren_expr_types{}.matches(matcher) && matcher.maybe_real(tk::RightParen);
    }
    return false;
}

std::unique_ptr<ParenExpr> ParenExpr::create(Parser& parser) {
    auto ptr = std::make_unique<ParenExpr>();

    Token first = parser.require_real(tk::LeftParen);
    ptr->m_Inner = paren_expr_types{}.create(parser);
    Token last = parser.require_real(tk::RightParen);

    ptr->post_init(first, last);
    ptr->m_Inner->set_parent(*ptr);

    return ptr;
}

void ParenExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Inner->visit(func);
}

void ParenExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Inner->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules
