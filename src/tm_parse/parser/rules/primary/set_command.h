//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class ObjectRef;
class PropertyAccess;
class Expr;

class SetCommand : public ParserRule {
   private:
    std::unique_ptr<ObjectRef> m_ObjectRef;
    std::unique_ptr<PropertyAccess> m_Property;
    std::unique_ptr<Expr> m_Expr;

   public:
    SetCommand() = default;
    ~SetCommand() override;

   public:
    SetCommand(const SetCommand&) = delete;
    SetCommand& operator=(const SetCommand&) = delete;
    SetCommand(SetCommand&&) noexcept ;
    SetCommand& operator=(SetCommand&&) noexcept ;

   public:
    const ObjectRef& object_ref() const noexcept { return *m_ObjectRef; };
    const PropertyAccess& property() const noexcept { return *m_Property; };
    const Expr& expr() const noexcept { return *m_Expr; };

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    RULE_STATIC_API(SetCommand);
};

}  // namespace tm_parse::rules