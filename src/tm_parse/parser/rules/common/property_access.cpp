//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/common/property_access.h"
#include "tm_parse/parser/rules/common/array_access.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"

namespace tm_parse::rules {

PropertyAccess::~PropertyAccess() = default;
PropertyAccess::PropertyAccess(PropertyAccess&&) noexcept = default;
PropertyAccess& PropertyAccess::operator=(PropertyAccess&&) noexcept = default;

bool PropertyAccess::matches(Matcher& matcher) noexcept {
    bool res = PropertyDotIdentifier::matches(matcher);
    ArrayAccess::matches(matcher);
    return res;
}

std::unique_ptr<PropertyAccess> PropertyAccess::create(Parser& parser) {
    auto rule = std::make_unique<PropertyAccess>();
    rule->m_Property = PropertyDotIdentifier::create(parser);
    rule->m_Property->set_parent(*rule);
    rule->copy_state(*rule->m_Property);
    return rule;
}

void PropertyAccess::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Property->visit(func);
}

void PropertyAccess::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Property->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules