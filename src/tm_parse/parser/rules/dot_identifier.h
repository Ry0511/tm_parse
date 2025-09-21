//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser_rule.h"
#include "tm_parse/parser/rule_factory.h"

namespace tm_parse {

class DotIdentifier : public ParserRule {
   public:
    DotIdentifier() = default;
    ~DotIdentifier() = default;

   public:
    RULE_FACTORY_STUBS(DotIdentifier);
};

}  // namespace tm_parse
