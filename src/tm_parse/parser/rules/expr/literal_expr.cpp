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

namespace {
constexpr tk::TokenKind
    literals[]{tk::Number, tk::None, tk::StringLiteral, tk::True, tk::False};
}

bool LiteralExpr::matches(Matcher& matcher) noexcept {
    return matcher.any_real(literals);
}

std::unique_ptr<LiteralExpr> LiteralExpr::create(Parser& parser) {
    auto rule = std::make_unique<LiteralExpr>();
    Token next = parser.require_any_real(literals);

    switch (next.Kind) {
        case tk::True:
        case tk::False: {
            rule->m_Value = (next == tk::True);
            break;
        }
        case tk::None: {
            rule->m_Value = NoneType{};
            break;
        }
        case tk::StringLiteral: {
            try {
                rule->m_Value = txt::sanitise_string(next.inner_text());
            } catch (const std::logic_error& err) {
                LOG_ERR("failed to sanitise string with error {}", err.what());
                rule->m_Value = std::monostate{};
            }
            break;
        }
        case tk::Number: {
            if (next.has_radix()) {
                auto res = txt::parse_double(next.text());
                if (res.has_value()) {
                    rule->m_Value = Number{res.value()};
                } else {
                    rule->m_Value = std::monostate{};
                }
            } else {
                auto res = txt::parse_int64(next.text());
                if (res.has_value()) {
                    rule->m_Value = Number{res.value()};
                } else {
                    rule->m_Value = std::monostate{};
                }
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
