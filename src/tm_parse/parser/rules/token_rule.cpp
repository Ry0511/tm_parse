//
// Date       : 24/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/token_rule.h"

namespace tm_parse {

bool TokenRule::matches(Parser&) noexcept {
    return true;
}

std::unique_ptr<TokenRule> TokenRule::create(Parser& parser) {
    TokenRule rule{};
    Token tk = parser.next_real();
    rule.post_init(tk, tk);
    return std::make_unique<TokenRule>(rule);
}

}  // namespace tm_parse
