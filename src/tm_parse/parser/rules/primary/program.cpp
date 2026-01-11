//
// Date       : 14/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/primary/program.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/util/text_helpers.h"

#include "tm_parse/parser/rules/expr/variable_expr.h"
#include "tm_parse/parser/rules/expr_type_list.h"
#include "tm_parse/parser/rules/pragmas/pragma_toggle.h"
#include "tm_parse/parser/rules/primary/mod_definition.h"
#include "tm_parse/parser/rules/primary/object_definition.h"
#include "tm_parse/parser/rules/primary/set_command.h"

namespace tm_parse::rules {

using program_child_rules = ExprTypeList<ObjectDefinition, SetCommand, PragmaToggle, VariableExpr>;

ProgramRule::ProgramRule() noexcept = default;
ProgramRule::~ProgramRule() noexcept = default;
ProgramRule::ProgramRule(ProgramRule&&) noexcept = default;
ProgramRule& ProgramRule::operator=(ProgramRule&&) noexcept = default;

bool ProgramRule::matches(Matcher& /*matcher*/) noexcept {
    return true;
}

std::unique_ptr<ProgramRule> ProgramRule::create(Parser& parser) {
    auto rule = std::make_unique<ProgramRule>();

    if (parser.peek_matches<PragmaToggle>()) {
        auto& child = rule->m_ChildRules.emplace_back(parser.create<PragmaToggle>());
        child->set_parent(*rule);
        if (child->as_ref<PragmaToggle>().type() != PragmaToggleType::CreateMod) {
            throw std::runtime_error{TXT("pragma toggle must be CreateMod")};
        }
    }

    // if enabled create_mod = ( ... ) should be the first rule
    if (parser.parse_state().AllowCreateMod) {
        auto& child = rule->m_ChildRules.emplace_back(parser.create<ModDefinition>());
        rule->m_ModDefinition = child->as<ModDefinition>();
        rule->m_ModDefinition->set_parent(*rule);
    }

    Matcher m = parser.create_matcher();
    while (!parser.is_eof()) {
        m.set_position(parser.position());

        if (m.match_real_seq<tk::Begin, tk::Object, tk::AnyIdentifier, tk::Equal>()) {
            auto& child = rule->m_ChildRules.emplace_back(parser.create<ObjectDefinition>());
            child->set_parent(*rule);

        } else {
            auto& child = rule->m_ChildRules.emplace_back(
                parser.create_one_of<SetCommand, PragmaToggle, VariableExpr>()
            );
            child->set_parent(*rule);

            if (auto* var = child->is<VariableExpr>()) {
                auto name = var->identifier().text();
                if (!rule->m_IdentifierList.insert(name, nullptr, var)) {
                    auto it = rule->m_IdentifierList.find(name);
                    throw std::runtime_error{
                        std::format(
                            TXT("attempting to overwrite\n{}with\n{}"),
                            it.Node->format_rule(),
                            var->format_rule()
                        )
                    };
                }
            }
        }
    }

    rule->post_init(*rule->m_ChildRules.front(), *rule->m_ChildRules.back());
    return rule;
}

void ProgramRule::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    for (const auto& rule : m_ChildRules) {
        rule->visit(func);
    }
}

void ProgramRule::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    for (const auto& rule : m_ChildRules) {
        rule->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
