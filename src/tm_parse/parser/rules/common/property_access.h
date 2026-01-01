//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class PropertyAccess : public ParserRule {
   private:
    std::unique_ptr<ParserRule> m_Property;

   public:
    PropertyAccess() = default;
    ~PropertyAccess() override;

   public:
    PropertyAccess(const PropertyAccess&) = delete;
    PropertyAccess& operator=(const PropertyAccess&) = delete;
    PropertyAccess(PropertyAccess&&) noexcept;
    PropertyAccess& operator=(PropertyAccess&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    const ParserRule& property() const noexcept { return *m_Property; }

   public:
    RULE_STATIC_API(PropertyAccess);
};

}  // namespace tm_parse::rules
