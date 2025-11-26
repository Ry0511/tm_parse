//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::rules {

bool LiteralExpr::matches(Matcher& matcher) noexcept {
    constexpr tk::TokenKind literals[]{tk::Number, tk::StringLiteral, tk::True, tk::False};
    return matcher.any_real(literals);
}

std::unique_ptr<LiteralExpr> LiteralExpr::create(Parser& parser) {
    auto rule = std::make_unique<LiteralExpr>();
    Token next = parser.next_real();

    switch (next.Kind) {
        case tk::True:
        case tk::False:
            rule->m_Value = (next == tk::True);
            break;

        case tk::StringLiteral:
            // TODO: Don't want this to kill the parsing if it is invalid
            rule->m_Value = txt::sanitise_string(next.inner_text());
            break;

        case tk::Number: {
            if (next.has_radix()) {
                rule->m_Value = txt::parse_double(next.text());
            } else {
                rule->m_Value = txt::parse_int64(next.text());
            }
            break;
        }

        default: {
            throw TokenError{"expecting one of [True, False, StringLiteral, Number]", next};
        }
    }

    rule->post_init(next);

    return rule;
}

LiteralExpr::~LiteralExpr() = default;

}  // namespace tm_parse::rules
