//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/dot_identifier.h"
#include "tm_parse/parser/rules/common/array_access.h"

namespace tm_parse::rules {

DotIdentifier::DotIdentifier() = default;
DotIdentifier::DotIdentifier(DotIdentifier&&) noexcept = default;
DotIdentifier& DotIdentifier::operator=(DotIdentifier&&) noexcept = default;
DotIdentifier::~DotIdentifier() noexcept = default;

const Token& DotIdentifierData::last_token() const noexcept {
    return ArrayPart ? ArrayPart->last_token() : IdentifierPart;
}

bool DotIdentifier::matches(Matcher& matcher) noexcept {
    if (!matcher.maybe_real(tk::AnyIdentifier)) {
        return false;
    }

    matcher.matches<ArrayAccess>();

    while (matcher.maybe(tk::Dot)) {
        if (!matcher.maybe(tk::AnyIdentifier)) {
            return false;
        }
        matcher.matches<ArrayAccess>();
    }

    return true;
}

std::unique_ptr<DotIdentifier> DotIdentifier::create(Parser& parser) {
    auto rule = std::make_unique<DotIdentifier>();

    do {
        DotIdentifierData& data = rule->m_Parts.emplace_back();
        data.IdentifierPart = parser.require_real(tk::AnyIdentifier);

        Matcher m = parser.create_matcher();
        if (m.matches<ArrayAccess>()) {
            data.ArrayPart = ArrayAccess::create(parser);
        }

    } while (parser.maybe_real(tk::Dot));

    rule->post_init(rule->first().IdentifierPart, rule->last().last_token());
    return rule;
}

}  // namespace tm_parse::rules