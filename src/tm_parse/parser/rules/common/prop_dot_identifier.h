//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class ArrayAccess;

struct PropertyDotIdentifierData {
    Token IdentifierPart;
    std::unique_ptr<ArrayAccess> ArrayPart;

    const Token& last_token() const noexcept;
};

class PropertyDotIdentifier : public ParserRule {
   private:
    std::vector<PropertyDotIdentifierData> m_Parts;

   public:
    PropertyDotIdentifier();
    ~PropertyDotIdentifier() noexcept override;

   public:
    PropertyDotIdentifier(const PropertyDotIdentifier&) = delete;
    PropertyDotIdentifier& operator=(const PropertyDotIdentifier&) = delete;
    PropertyDotIdentifier(PropertyDotIdentifier&&) noexcept;
    PropertyDotIdentifier& operator=(PropertyDotIdentifier&&) noexcept;

   public:
    const std::vector<PropertyDotIdentifierData>& identifier_parts() const noexcept {
        return m_Parts;
    }
    const PropertyDotIdentifierData& first() const noexcept { return m_Parts.front(); }
    const PropertyDotIdentifierData& last() const noexcept { return m_Parts.back(); }
    size_t size() const noexcept { return m_Parts.size(); }

   public:
    RULE_STATIC_API(PropertyDotIdentifier);
};

}  // namespace tm_parse::rules
