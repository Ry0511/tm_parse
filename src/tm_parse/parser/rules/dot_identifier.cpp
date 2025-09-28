//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/dot_identifier.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse::rules {

bool DotIdentifier::matches(Parser& parser) noexcept {
    // TODO: To implement these properly and efficently we need to implement look-ahead
    //  functionality. The simplest approach is to just copy the lexer and then use that to process
    //  tokens without modifying the current lexer. The issue with that is that we are doing more
    //  work than we need to.
    return true;
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

}  // namespace tm_parse::rules