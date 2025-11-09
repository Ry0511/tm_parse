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

bool Expr::matches(Matcher& matcher) noexcept {
    // clang-format off
    return matcher.matches<MetaVarExpr>()
           || matcher.matches<ParenExpr>()
           || matcher.matches<AssignmentExprList>()
           || matcher.matches<AssignmentExpr>()
           || matcher.matches<ComposedExpr>()
           || matcher.matches<LiteralExpr>()
           || matcher.matches<IdentifierRefExpr>();
    // clang-format on
}

std::unique_ptr<ParserRule> Expr::create(Parser& parser) {
    Matcher m = parser.create_matcher();

#define TRY_CREATE_RULE(rule)        \
    if (m.matches<rule>()) {         \
        return rule::create(parser); \
    }

    TRY_CREATE_RULE(MetaVarExpr);
    TRY_CREATE_RULE(AssignmentExprList);
    TRY_CREATE_RULE(ParenExpr);
    TRY_CREATE_RULE(AssignmentExpr);
    TRY_CREATE_RULE(ComposedExpr);
    TRY_CREATE_RULE(LiteralExpr);
    TRY_CREATE_RULE(IdentifierRefExpr);

#undef TRY_CREATE_RULE

    throw TokenError("could not create any expression", m.next_real());
}

}  // namespace tm_parse::rules
