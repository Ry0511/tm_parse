//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

// TODO: Originally I thought that a rule satisfying:
//    Identifier ( Dot Identifier )*
//  would have been enough but when creating some example programs I noticed that there was/could
//  be a need for some more extended variants such as the following
//    $(Globals.ExpScaleByLevelDifference(0).HigherLevelEnemyExpScale)
//  which I don't need to explain what it does.
//

class DotIdentifier : public ParserRule {
   private:
    std::vector<TextRegion> m_NameParts;

   public:
    DotIdentifier() = default;
    ~DotIdentifier() override;

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
