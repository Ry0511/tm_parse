//
// Date       : 16/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/expr/assignment_expr.h"
#include "tm_parse/parser/rules/expr/assignment_expr_list.h"
#include "tm_parse/parser/rules/expr/composed_expr.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"
#include "tm_parse/parser/rules/expr/log_info_expr.h"
#include "tm_parse/parser/rules/expr/meta_var_expr.h"
#include "tm_parse/parser/rules/expr/paren_expr.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"
#include "tm_parse/parser/rules/expr_type_list.h"

namespace tm_parse::rules {

// clang-format off

using all_expressions = ExprTypeList<
    MetaVarExpr,
    ParenExpr,
    AssignmentExprList,
    AssignmentExpr,
    ComposedExpr,
    LiteralExpr,
    UnquotedStrLiteral
>;

using safe_expressions = ExprTypeList<
    MetaVarExpr,
    ParenExpr,
    AssignmentExprList,
    AssignmentExpr,
    ComposedExpr,
    LiteralExpr
>;

// What is allowed on the right hand side of an assignment
// i.e.,
//   A = EXPR
//
using assignment_expr_types = ExprTypeList<
    MetaVarExpr,
    ParenExpr,
    AssignmentExprList,
    ComposedExpr,
    LiteralExpr,
    UnquotedStrLiteral
>;

// What is allowed on the right hand side of an assignment inside of an expression list?
// i.e.,
//   ( A = EXPR, B = EXPR )
//
using assignment_expr_list_types = ExprTypeList<
    MetaVarExpr,
    ParenExpr,
    AssignmentExprList,
    ComposedExpr,
    LiteralExpr
>;

// clang-format on

}  // namespace tm_parse::rules