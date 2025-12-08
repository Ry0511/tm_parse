//
// Date       : 02/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/parser/rules/pragmas/pragma_toggle.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/unquoted_str_literal.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::rules {

namespace {

constexpr tk::TokenKind toggle_type[]{tk::Enable, tk::Disable};

struct Mapping {
    str_view Text;
    PragmaToggleType Type;
};

constexpr Mapping all_mappings[]{
    {TXT("unquotedliterals"), PragmaToggleType::UnquotedLiterals},
    {      TXT("create_mod"),        PragmaToggleType::CreateMod},
};

}  // namespace

void PragmaToggle::toggle_for(Parser& parser) const noexcept {
    switch (m_Type) {
        case PragmaToggleType::UnquotedLiterals:
            parser.parse_state().AllowUnquotedStrings = m_State;
            break;

        case PragmaToggleType::CreateMod:
            parser.parse_state().AllowCreateMod = m_State;
            break;

        default:
            break;
    }
}

bool PragmaToggle::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::Pragma)
           && matcher.any(toggle_type)
           && matcher.maybe(tk::AnyIdentifier);
}

std::unique_ptr<PragmaToggle> PragmaToggle::create(Parser& parser) {
    const Token& first = parser.require_real(tk::Pragma);
    const Token& val = parser.require_any(toggle_type);
    const Token& last = parser.require(tk::AnyIdentifier);

    auto rule = std::make_unique<PragmaToggle>();
    rule->m_State = (val == tk::Enable);
    rule->post_init(first, last);

    for (const auto& mapping : all_mappings) {
        if (txt::equal_icase(last.text(), mapping.Text)) {
            rule->m_Type = mapping.Type;
            break;
        }
    }

    rule->toggle_for(parser);

    return rule;
}

}  // namespace tm_parse::rules
