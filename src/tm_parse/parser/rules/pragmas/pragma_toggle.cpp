//
// Date       : 02/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/parser/rules/pragmas/pragma_toggle.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"

namespace tm_parse::rules {

namespace {
constexpr tk::TokenKind toggle_type[]{tk::Enable, tk::Disable};
}

void PragmaToggle::toggle_for(Parser& parser) const noexcept {
    if (m_Type == PragmaToggleType::UnquotedLiterals) {
        parser.parse_state().AllowUnquotedStrings = m_State;
    }
}

bool PragmaToggle::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::Pragma) && matcher.any(toggle_type)
           && matcher.maybe(tk::AnyIdentifier);
}

std::unique_ptr<PragmaToggle> PragmaToggle::create(Parser& parser) {
    const Token& first = parser.require_real(tk::Pragma);
    const Token& val = parser.require_any(toggle_type);
    const Token& last = parser.require(tk::AnyIdentifier);

    auto rule = std::make_unique<PragmaToggle>();
    rule->m_State = (val == tk::Enable);
    rule->post_init(first, last);

    if (txt::equal_icase(last.text(), TXT("unquotedliterals"))) {
        rule->m_Type = PragmaToggleType::UnquotedLiterals;
    }

    rule->toggle_for(parser);

    return rule;
}

}  // namespace tm_parse::rules
