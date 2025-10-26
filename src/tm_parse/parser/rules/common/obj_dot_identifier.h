//
// Date       : 26/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class ObjectDotIdentifier : public ParserRule {
   private:
    std::vector<Token> m_Parts;

   public:
    ObjectDotIdentifier() noexcept = default;
    ~ObjectDotIdentifier() noexcept override = default;

   public:
    ObjectDotIdentifier(const ObjectDotIdentifier&) noexcept = default;
    ObjectDotIdentifier& operator=(const ObjectDotIdentifier&) noexcept = default;
    ObjectDotIdentifier(ObjectDotIdentifier&&) noexcept = default;
    ObjectDotIdentifier& operator=(ObjectDotIdentifier&&) noexcept = default;

   public:
    const std::vector<Token>& identifiers() const noexcept { return m_Parts; }

   public:
    RULE_STATIC_API(ObjectDotIdentifier);
};

}  // namespace tm_parse::rules
