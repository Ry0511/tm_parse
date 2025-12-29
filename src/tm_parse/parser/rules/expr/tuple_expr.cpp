//
// Date       : 25/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/tuple_expr.h"
#include "tm_parse/parser/rules/util/common_expr.h"

namespace tm_parse::rules {

bool TupleExpr::matches(Matcher& matcher) noexcept {
    if (!matcher.maybe_real(tk::LeftParen)) {
        return false;
    }

    assignment_expr_list_types exprs{};
    int expr_count = 0;
    while (exprs.matches(matcher)) {
        matcher.maybe_real(tk::Comma);
        expr_count++;
    }

    // (A, B)
    return expr_count > 1 && matcher.maybe_real(tk::RightParen);
}

std::unique_ptr<TupleExpr> TupleExpr::create(Parser& parser) {
    Token first = parser.require_real(tk::LeftParen);
    auto rule = std::make_unique<TupleExpr>();
    assignment_expr_list_types exprs{};

    do {
        auto& expr = rule->m_Literals.emplace_back(exprs.create(parser));
        expr->set_parent(*rule);
    } while (parser.maybe_real(tk::Comma));

    Token last = parser.require_real(tk::RightParen);
    rule->post_init(first, last);
    return rule;
}

void TupleExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    for (const auto& expr : m_Literals) {
        expr->visit(func);
    }
}

void TupleExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    for (auto& expr : m_Literals) {
        expr->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
