//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "parser_rule.h"

namespace tm_parse::rules {

class ObjectRef;
class PropertyAccess;

class SetCommand : public ParserRule {
   private:
    std::unique_ptr<ObjectRef> m_ObjectRef;
    std::unique_ptr<PropertyAccess> m_Property;

   public:
    SetCommand() = default;
    ~SetCommand() override = default;

   public:
    SetCommand(const SetCommand&) = delete;
    SetCommand& operator=(const SetCommand&) = delete;
    SetCommand(SetCommand&&) = default;
    SetCommand& operator=(SetCommand&&) = default;

   public:
    const ObjectRef& object_ref() const noexcept { return *m_ObjectRef; };
    const PropertyAccess& property() const noexcept { return *m_Property; };

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

   public:
    RULE_STATIC_API(SetCommand);
};

}  // namespace tm_parse::rules