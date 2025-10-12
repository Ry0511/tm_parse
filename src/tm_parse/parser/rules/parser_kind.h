//
// Date       : 12/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::rkind {

using parser_rule_kind_int = uint8_t;

enum ParserRuleKind : parser_rule_kind_int {
    ArrayAccess,
    DotIdentifier,
    ObjectRef,
    PropertyAccess,

    ObjectDefinition,
    SetCommand,

    AssignmentExpr,
    AssignmentExprList,
    ComposedExpr,
    BinaryOpExpr,
    UnaryOpExpr,
    Expr,
    IdentifierRefExpr,
    LiteralExpr,
    LogInfoExpr,
    MetaVarExpr,
    ParenExpr,

    TokenRule, // potentially planned for removal
};

}  // namespace tm_parse::rkind
