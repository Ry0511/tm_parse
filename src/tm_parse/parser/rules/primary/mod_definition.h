//
// Date       : 14/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class AssignmentExprList;

class ModDefinition : public ParserRule {
   private:
    std::unique_ptr<AssignmentExprList> m_InitArgs;

   public:
    explicit ModDefinition() noexcept;
    ~ModDefinition() noexcept override;
    ModDefinition(const ModDefinition&) = delete;
    ModDefinition& operator=(const ModDefinition&) = delete;
    ModDefinition(ModDefinition&&) noexcept;
    ModDefinition& operator=(ModDefinition&&) noexcept;

   public:
    const AssignmentExprList& expr_list() const noexcept { return *m_InitArgs; }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(ModDefinition);
};

}  // namespace tm_parse::rules
