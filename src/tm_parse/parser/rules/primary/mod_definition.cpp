//
// Date       : 14/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/assignment_expr_list.h"
#include "tm_parse/parser/rules/primary/mod_definition.h"

namespace tm_parse::rules {

ModDefinition::ModDefinition() noexcept = default;
ModDefinition::~ModDefinition() noexcept = default;
ModDefinition::ModDefinition(ModDefinition&&) noexcept = default;
ModDefinition& ModDefinition::operator=(ModDefinition&&) noexcept = default;

bool ModDefinition::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::CreateMod)
           && matcher.maybe_real(tk::Equal)
           && matcher.matches<AssignmentExprList>();
}

std::unique_ptr<ModDefinition> ModDefinition::create(Parser& parser) {
    Token first = parser.require_real(tk::CreateMod);
    parser.require_real(tk::Equal);

    auto rule = std::make_unique<ModDefinition>();
    rule->m_InitArgs = AssignmentExprList::create(parser);
    rule->m_InitArgs->set_parent(*rule);
    rule->post_init(first, rule->m_InitArgs->last_token());

    return rule;
}

void ModDefinition::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_InitArgs->visit(func);
}

void ModDefinition::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_InitArgs->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules
