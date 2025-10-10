//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "property_access.h"

#include "array_access.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/dot_identifier.h"

namespace tm_parse::rules {

PropertyAccess::~PropertyAccess() = default;
PropertyAccess::PropertyAccess(PropertyAccess&&) noexcept = default;
PropertyAccess& PropertyAccess::operator=(PropertyAccess&&) noexcept = default;

bool PropertyAccess::matches(Matcher& matcher) noexcept {
    bool res = DotIdentifier::matches(matcher);
    ArrayAccess::matches(matcher);
    return res;
}

std::unique_ptr<PropertyAccess> PropertyAccess::create(Parser& parser) {
    auto rule = std::make_unique<PropertyAccess>();

    rule->m_Property = DotIdentifier::create(parser);
    rule->m_Property->set_parent(*rule);

    Matcher matcher = parser.create_matcher();
    if (ArrayAccess::matches(matcher)) {
        rule->m_ArrayAccess = ArrayAccess::create(parser);
        rule->m_ArrayAccess->set_parent(*rule);
    }

    rule->copy_state(*rule->m_Property);

    return rule;
}

void PropertyAccess::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    if (m_ArrayAccess != nullptr) {
        m_ArrayAccess->visit(func);
    }
}

void PropertyAccess::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Property->cascade_assign_parents(this);

    if (m_ArrayAccess != nullptr) {
        m_ArrayAccess->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules