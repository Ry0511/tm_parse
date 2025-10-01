//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/property_access.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/dot_identifier.h"
#include "tm_parse/parser/rules/array_access.h"

namespace tm_parse::rules {

bool PropertyAccess::matches(Matcher& matcher) noexcept {
    bool res = DotIdentifier::matches(matcher);
    ArrayAccess::matches(matcher);
    return res;
}

std::unique_ptr<PropertyAccess> PropertyAccess::create(Parser& parser) {
    PropertyAccess rule{};

    rule.m_Property = DotIdentifier::create(parser);

    Matcher matcher = parser.create_matcher();
    if (ArrayAccess::matches(matcher)) {
        rule.m_ArrayAccess = ArrayAccess::create(parser);
    }

    rule.copy_state(*rule.m_Property);
    return std::make_unique<PropertyAccess>(std::move(rule));
}

void PropertyAccess::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    if (m_ArrayAccess != nullptr) {
        m_ArrayAccess->visit(func);
    }
}

}  // namespace tm_parse::rules