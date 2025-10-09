//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/paren_expr.h"

namespace tm_parse::rules {

bool ParenExpr::matches(Matcher& matcher) noexcept {
    if (matcher.maybe_real(tk::LeftParen)) {
        return Expr::matches(matcher) && matcher.maybe_real(tk::RightParen);
    }
    return false;
}

std::unique_ptr<ParenExpr> ParenExpr::create(Parser& parser) {
    auto ptr = std::make_unique<ParenExpr>();

    Token first = parser.require_next_real(tk::LeftParen);
    ptr->m_Inner = Expr::create(parser);
    Token last = parser.require_next_real(tk::RightParen);

    ptr->post_init(first, last);
    ptr->m_Inner->set_parent(*ptr);

    return ptr;
}

ParenExpr::~ParenExpr() = default;
ParenExpr::ParenExpr(ParenExpr&&) noexcept = default;
ParenExpr& ParenExpr::operator=(ParenExpr&&) noexcept = default;

void ParenExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Inner->visit(func);
}

}  // namespace tm_parse::rules
