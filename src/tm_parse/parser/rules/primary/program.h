//
// Date       : 14/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"
#include "tm_parse/parser/scoped_identifier_list.h"

namespace tm_parse::rules {

class ModDefinition;
class SetCommand;
class ObjectDefinitionRule;

// Going to keep the Rule suffix here but all other rules don't use it
class ProgramRule : public ParserRule {
   private:
    ModDefinition* m_ModDefinition{nullptr};
    std::vector<std::unique_ptr<ParserRule>> m_ChildRules;
    ScopedIdentifierList m_IdentifierList{};

   public:
    explicit ProgramRule() noexcept;
    ~ProgramRule() noexcept override;
    ProgramRule(const ProgramRule&) = delete;
    ProgramRule& operator=(const ProgramRule&) = delete;
    ProgramRule(ProgramRule&&) noexcept;
    ProgramRule& operator=(ProgramRule&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    const ModDefinition* mod_definition() const noexcept { return m_ModDefinition; }
    const auto& child_rules() const noexcept { return m_ChildRules; }
    void simplify_ast() noexcept override;

   public:
    RULE_STATIC_API(ProgramRule);
};

}  // namespace tm_parse::rules
