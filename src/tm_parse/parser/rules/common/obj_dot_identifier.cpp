//
// Date       : 26/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/common/obj_dot_identifier.h"

#include "tm_parse/parser/parser.h"

namespace tm_parse::rules {

bool ObjectDotIdentifier::matches(Matcher& matcher) noexcept {
    if (!matcher.maybe_real(tk::AnyIdentifier)) {
        return false;
    }

    while (matcher.maybe(tk::Dot)) {
        if (!matcher.maybe(tk::AnyIdentifier)) {
            return false;
        }
    }

    return true;
}

std::unique_ptr<ObjectDotIdentifier> ObjectDotIdentifier::create(Parser& parser) {
    auto rule = std::make_unique<ObjectDotIdentifier>();

    rule->m_Parts.push_back(parser.require_real(tk::AnyIdentifier));

    while (parser.maybe(tk::Dot)) {
        rule->m_Parts.push_back(parser.require(tk::AnyIdentifier));
    }

    rule->post_init(rule->m_Parts.front(), rule->m_Parts.back());
    return rule;
}

}  // namespace tm_parse::rules
