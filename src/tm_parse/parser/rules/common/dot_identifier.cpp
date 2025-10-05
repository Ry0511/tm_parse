//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "dot_identifier.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse::rules {

bool DotIdentifier::matches(Matcher& matcher) noexcept {
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

std::unique_ptr<DotIdentifier> DotIdentifier::create(Parser& parser) {
    auto rule = std::make_unique<DotIdentifier>();
    Token first = parser.require_next_real(tk::AnyIdentifier);
    Token last = first;

    rule->m_NameParts.push_back(first.Region);

    while (parser.maybe(tk::Dot)) {
        last = parser.require(tk::AnyIdentifier);
        rule->m_NameParts.push_back(last.Region);
    }

    rule->m_NameParts.shrink_to_fit();
    rule->post_init(first, last);

    return rule;
}

DotIdentifier::~DotIdentifier() = default;

}  // namespace tm_parse::rules