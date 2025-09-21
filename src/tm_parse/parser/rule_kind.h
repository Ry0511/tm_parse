//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::rules {

using rule_kind_int = int;

enum RuleKind : rule_kind_int {
    DotIdentifier,     // Foo.Baz.Bar
    ObjectReference,   // Foo.Baz:Bar.Baz
    ArrayAccess,       // (0) | [0]
    PropertyAccess,    // Property ArrayAccess?
    SetCommand,        // set ObjectReference Identifier Expression
    ObjectDefinition,  // Begin Object Class=? Name=? ... End Object
};

}  // namespace tm_parse::rules
