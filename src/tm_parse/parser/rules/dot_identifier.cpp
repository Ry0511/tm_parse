//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/dot_identifier.h"

namespace tm_parse {

bool DotIdentifier::matches(Parser& parser) noexcept {
    return false;
}

ParseResult<DotIdentifier> DotIdentifier::create(Parser& parser) noexcept {
    return ParseResult<DotIdentifier>::fail({});
}

}  // namespace tm_parse