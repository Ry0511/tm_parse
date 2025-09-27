//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/dot_identifier.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse {

bool DotIdentifier::matches(Parser& parser) noexcept {
    return parser.maybe(tk::Identifier);
}

std::unique_ptr<DotIdentifier> DotIdentifier::create(Parser& parser) {
    DotIdentifier rule{};
    Token first = parser.require_next_real(tk::AnyIdentifier);
    Token last = first;

    while (parser.maybe(tk::Dot)) {
        last = parser.require(tk::AnyIdentifier);
    }

    rule.post_init(first, last);
    return std::make_unique<DotIdentifier>(rule);
}

}  // namespace tm_parse