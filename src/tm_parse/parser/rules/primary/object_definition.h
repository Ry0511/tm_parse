//
// Date       : 05/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class AssignmentExpr;
class ObjectDotIdentifier;

class ObjectDefinition : public ParserRule {
   private:
    std::unique_ptr<ObjectDotIdentifier> m_ClassName;
    std::unique_ptr<ObjectDotIdentifier> m_ObjectName;
    std::vector<std::unique_ptr<ParserRule>> m_ChildRules;

   public:
    ObjectDefinition() noexcept;
    ~ObjectDefinition() noexcept override;

   public:
    ObjectDefinition(const ObjectDefinition&) = delete;
    ObjectDefinition& operator=(const ObjectDefinition&) = delete;
    ObjectDefinition(ObjectDefinition&&) noexcept;
    ObjectDefinition& operator=(ObjectDefinition&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    RULE_STATIC_API(ObjectDefinition);
};

}  // namespace tm_parse::rules
