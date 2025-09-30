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
   private:
    std::vector<TextRegion> m_NameParts;

   public:
    DotIdentifier() = default;
    ~DotIdentifier() override = default;

   public:
    DotIdentifier(const DotIdentifier&) = default;
    DotIdentifier& operator=(const DotIdentifier&) = default;
    DotIdentifier(DotIdentifier&&) = default;
    DotIdentifier& operator=(DotIdentifier&&) = default;

   public:
    const std::vector<TextRegion>& name_parts() const noexcept { return m_NameParts; }

   public:
    RULE_STATIC_API(DotIdentifier);
};

}  // namespace tm_parse::rules
