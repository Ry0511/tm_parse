//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser_rule.h"

namespace tm_parse::rules {

class DotIdentifier : public ParserRule {
   public:
    DotIdentifier() = default;
    ~DotIdentifier() override = default;

   public:
    str rule_name() const noexcept override { return "DotIdentifier"; }

   public:
    RULE_STATIC_API(DotIdentifier);
};

}  // namespace tm_parse
