//
// Date       : 12/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::rkind {

using parser_rule_kind_int = uint8_t;

// TODO: this can be replaced with a static variable on the class
enum ParserRuleKind : parser_rule_kind_int {
    ArrayAccess,
    ObjectDotIdentifier,
    PropertyDotIdentifier,
    ObjectRef,
    PropertyAccess,
    ClassObjectRef,
    FullObjectRef,

    ProgramRule,
    ModDefinition,
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
    TupleExpr,
    LogInfoExpr,
    MetaVarExpr,
    ParenExpr,
    UnquotedStrLiteral,
    VariableExpr,

    PragmaToggle,

    TokenRule,  // planned for removal
    Unknown,
    ENUM_COUNT,
};

constexpr parser_rule_kind_int parser_rule_kind_count{ParserRuleKind::ENUM_COUNT};

}  // namespace tm_parse::rkind
