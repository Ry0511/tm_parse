//
// Date       : 05/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class AssignmentExpr;

class ObjectDefinition : public ParserRule {
   private:
    str m_ClassName;
    str m_ObjectName;
    std::vector<std::unique_ptr<ObjectDefinition>> m_ChildObjects;
    std::vector<std::unique_ptr<AssignmentExpr>> m_PropertyWrites;

   public:
    ObjectDefinition() = default;
    ~ObjectDefinition() override = default;

   public:
    ObjectDefinition(const ObjectDefinition&) = default;
    ObjectDefinition& operator=(const ObjectDefinition&) = default;
    ObjectDefinition(ObjectDefinition&&) = default;
    ObjectDefinition& operator=(ObjectDefinition&&) = default;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

    RULE_STATIC_API(ObjectDefinition);
};

}  // namespace tm_parse::rules
