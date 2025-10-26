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

class ArrayAccess;

// TODO: There always was and is a plan to introduce a NameContext or something similar to resolve
//  duplicated identifiers but currently that does not exist so just going to use Token as a
//  placeholder until that is ready.
struct DotIdentifierData {
    Token IdentifierPart;
    std::unique_ptr<ArrayAccess> ArrayPart;

    const Token& last_token() const noexcept;
};

class DotIdentifier : public ParserRule {
   private:
    std::vector<DotIdentifierData> m_Parts;

   public:
    DotIdentifier();
    ~DotIdentifier() noexcept override;

   public:
    DotIdentifier(const DotIdentifier&) = delete;
    DotIdentifier& operator=(const DotIdentifier&) = delete;
    DotIdentifier(DotIdentifier&&) noexcept;
    DotIdentifier& operator=(DotIdentifier&&) noexcept;

   public:
    const std::vector<DotIdentifierData>& identifier_parts() const noexcept { return m_Parts; }
    const DotIdentifierData& first() const noexcept { return m_Parts.front(); }
    const DotIdentifierData& last() const noexcept { return m_Parts.back(); }
    size_t size() const noexcept { return m_Parts.size(); }

   public:
    RULE_STATIC_API(DotIdentifier);
};

}  // namespace tm_parse::rules
