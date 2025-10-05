//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/Expr.h"

#include "tm_parse/lexer/token_error.h"

#include "tm_parse/parser/rules/expr/assignment_expr.h"
#include "tm_parse/parser/rules/expr/assignment_expr_list.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"
#include "tm_parse/parser/rules/expr/paren_expr.h"

namespace tm_parse::rules {

namespace {

template <class T>
std::unique_ptr<T> match_create(Parser& parser) {
    Matcher matcher = parser;
    if (T::matches(matcher)) {
        return T::create(parser);
    }
    return nullptr;
}

}  // namespace

bool Expr::matches(Matcher& matcher) noexcept {
#define TRY_MATCH_RULE(rule)                    \
    {                                           \
        Matcher m = matcher;                    \
        if (rule::matches(m)) {                 \
            matcher.set_position(m.position()); \
            return true;                        \
        }                                       \
    }

    TRY_MATCH_RULE(AssignmentExprList);
    TRY_MATCH_RULE(ParenExpr);
    TRY_MATCH_RULE(AssignmentExpr);
    TRY_MATCH_RULE(LiteralExpr);
}

std::unique_ptr<Expr> Expr::create(Parser& parser) {
    Matcher m = parser;

#define TRY_CREATE_RULE(rule)                    \
    if (auto ptr = match_create<rule>(parser)) { \
        return ptr;                              \
    }

    TRY_CREATE_RULE(AssignmentExprList);
    TRY_CREATE_RULE(ParenExpr);
    TRY_CREATE_RULE(AssignmentExpr);
    TRY_CREATE_RULE(LiteralExpr);

    throw TokenError("could not create any expression", m.next_real());
}

}  // namespace tm_parse::rules
