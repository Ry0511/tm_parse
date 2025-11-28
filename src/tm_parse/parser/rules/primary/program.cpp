//
// Date       : 14/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/primary/program.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/primary/mod_definition.h"
#include "tm_parse/parser/rules/primary/object_definition.h"
#include "tm_parse/parser/rules/primary/set_command.h"

namespace tm_parse::rules {

ProgramRule::ProgramRule() noexcept = default;
ProgramRule::~ProgramRule() noexcept = default;
ProgramRule::ProgramRule(ProgramRule&&) noexcept = default;
ProgramRule& ProgramRule::operator=(ProgramRule&&) noexcept = default;

bool ProgramRule::matches(Matcher& matcher) noexcept {
    if (!matcher.matches<ModDefinition>()) {
        return false;
    }

    while (!matcher.is_eof()) {
        if (!matcher.matches<ObjectDefinition>() && !matcher.matches<SetCommand>()) {
            return false;
        }
    }

    return true;
}

std::unique_ptr<ProgramRule> ProgramRule::create(Parser& parser) {
    auto rule = std::make_unique<ProgramRule>();
    rule->m_ModDefinition = parser.create<ModDefinition>();
    rule->m_ModDefinition->set_parent(*rule);

    while (!parser.is_eof()) {
        const Token& tok = parser.peek_real();

        switch (tok.Kind) {
            case tk::Begin: {
                auto& child = rule->m_BodyRules.emplace_back(parser.create<ObjectDefinition>());
                child->set_parent(*rule);
                break;
            }
            case tk::Set: {
                auto& child = rule->m_BodyRules.emplace_back(parser.create<SetCommand>());
                child->set_parent(*rule);
                break;
            }
            default: {
                throw TokenError{"unknown token in ProgramRule", tok};
            }
        }
    }

    if (!rule->m_BodyRules.empty()) {
        rule->post_init(*rule->m_ModDefinition, *rule->m_BodyRules.back());
    } else {
        rule->post_init(*rule->m_ModDefinition, *rule->m_ModDefinition);
    }

    return rule;
}

void ProgramRule::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_ModDefinition->visit(func);
    for (const auto& rule : m_BodyRules) {
        rule->visit(func);
    }
}

void ProgramRule::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_ModDefinition->cascade_assign_parents(this);
    for (const auto& rule : m_BodyRules) {
        rule->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
