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
#include "tm_parse/parser/rules/expr/tuple_expr.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"
#include "tm_parse/parser/rules/expr_type_list.h"
#include "tm_parse/parser/rules/common/class_obj_ref.h"

namespace tm_parse::rules {

// All expressions in order of precedence.
using all_expressions = ExprTypeList<
    MetaVarExpr,
    ComposedExpr,
    AssignmentExprList,
    AssignmentExpr,
    LiteralExpr,
    TupleExpr,
    UnquotedStrLiteral>;

// TODO: Remove this and remove ParenExpr
using paren_expr_types = ExprTypeList<
    MetaVarExpr,
    AssignmentExprList,
    AssignmentExpr,
    LiteralExpr>;

//
// What is allowed on the right hand side of an assignment
// i.e.,
//   A = EXPR
//
using assignment_expr_types = ExprTypeList<
    MetaVarExpr,
    ComposedExpr,
    AssignmentExprList,
    LiteralExpr,
    TupleExpr,
    UnquotedStrLiteral,
    ClassObjectRef>;

//
// What is allowed on the right hand side of an assignment inside of an expression list
// i.e.,
//   ( A = EXPR, B = EXPR )
//
using assignment_expr_list_types = ExprTypeList<
    MetaVarExpr,
    ComposedExpr,
    AssignmentExprList,
    TupleExpr,
    LiteralExpr,
    ClassObjectRef>;

//
// What is allowed on the right hand side of an variable assignment expression
// i.e.,
//   let identifier = EXPR
//
using variable_expr_types = ExprTypeList<
    ComposedExpr,
    LiteralExpr,
    UnquotedStrLiteral,
    ClassObjectRef>;

}  // namespace tm_parse::rules