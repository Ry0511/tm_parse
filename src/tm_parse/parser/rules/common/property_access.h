//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "../parser_rule.h"

namespace tm_parse::rules {

class DotIdentifier;
class ArrayAccess;

class PropertyAccess : public ParserRule {
   private:
    std::unique_ptr<DotIdentifier> m_Property;   // mandatory
    std::unique_ptr<ArrayAccess> m_ArrayAccess;  // nullable

   public:
    PropertyAccess() = default;
    ~PropertyAccess() override;

   public:
    PropertyAccess(const PropertyAccess&) = delete;
    PropertyAccess& operator=(const PropertyAccess&) = delete;
    PropertyAccess(PropertyAccess&&);
    PropertyAccess& operator=(PropertyAccess&&);

   public:
    const DotIdentifier& property() const noexcept { return *m_Property; }
    const ArrayAccess* array_access() const noexcept { return m_ArrayAccess.get(); }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

   public:
    RULE_STATIC_API(PropertyAccess);
};

}  // namespace tm_parse::rules
