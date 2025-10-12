//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/expr.h"

#include "tm_parse/lexer/token_error.h"

#include "tm_parse/parser/rules/expr/assignment_expr.h"
#include "tm_parse/parser/rules/expr/assignment_expr_list.h"
#include "tm_parse/parser/rules/expr/composed_expr.h"
#include "tm_parse/parser/rules/expr/identifier_ref_expr.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"
#include "tm_parse/parser/rules/expr/log_info_expr.h"
#include "tm_parse/parser/rules/expr/meta_var_expr.h"
#include "tm_parse/parser/rules/expr/paren_expr.h"

namespace tm_parse::rules {

namespace {

template <class T>
std::unique_ptr<T> match_create(Parser& parser) {
    Matcher matcher = parser.create_matcher();
    if (T::matches(matcher)) {
        return T::create(parser);
    }
    return nullptr;
}

}  // namespace

bool Expr::matches(Matcher& matcher) noexcept {
#define TRY_MATCH_RULE(rule)      \
    pos = matcher.position();     \
    if (rule::matches(matcher)) { \
        return true;              \
    }                             \
    matcher.set_position(pos);

    size_t pos = matcher.position();

    TRY_MATCH_RULE(LogInfoExpr);
    TRY_MATCH_RULE(MetaVarExpr);
    TRY_MATCH_RULE(ParenExpr);
    TRY_MATCH_RULE(AssignmentExprList);
    TRY_MATCH_RULE(AssignmentExpr);
    TRY_MATCH_RULE(ComposedExpr);
    TRY_MATCH_RULE(LiteralExpr);
    TRY_MATCH_RULE(IdentifierRefExpr);

    return false;
}

std::unique_ptr<Expr> Expr::create(Parser& parser) {
    Matcher m = parser.create_matcher();

#define TRY_CREATE_RULE(rule)                    \
    if (auto ptr = match_create<rule>(parser)) { \
        return ptr;                              \
    }

    if (auto ptr = match_create<LogInfoExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<MetaVarExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<ParenExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<AssignmentExprList>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<AssignmentExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<ComposedExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<LiteralExpr>(parser)) {
        return ptr;
    }

    if (auto ptr = match_create<IdentifierRefExpr>(parser)) {
        return ptr;
    }

    throw TokenError("could not create any expression", m.next_real());
}

Expr::~Expr() = default;

}  // namespace tm_parse::rules
