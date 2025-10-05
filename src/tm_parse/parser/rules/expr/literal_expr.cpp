//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"

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
            rule->m_Value = str{next.inner_text()};
            break;

        case tk::Number: {
            // TODO: Replace calls of strto with our own wrappers. Also handle the potential errors.
            str_view number_text = next.text();
            if (next.has_radix()) {
                // NOLINTNEXTLINE
                rule->m_Value = static_cast<double>(std::strtod(number_text.data(), nullptr));
            } else {
                // NOLINTNEXTLINE
                rule->m_Value = static_cast<int64_t>(std::strtoll(number_text.data(), nullptr, 10));
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
