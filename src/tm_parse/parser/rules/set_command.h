//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser_rule.h"

namespace tm_parse::rules {

class ObjectRef;
class DotIdentifier;

class SetCommand : public ParserRule {
   private:
    std::unique_ptr<ObjectRef> m_ObjectRef;
    std::unique_ptr<DotIdentifier> m_Property;

   public:
    SetCommand() = default;
    ~SetCommand() override = default;

   public:
    SetCommand(const SetCommand&) = delete;
    SetCommand& operator=(const SetCommand&) = delete;
    SetCommand(SetCommand&&) = default;
    SetCommand& operator=(SetCommand&&) = default;

   public:
    str rule_name() const noexcept override { return "SetCommand"; }
    void visit(const std::function<bool(const ParserRule&)>& func) const noexcept override;

   public:
    RULE_STATIC_API(SetCommand);
};

}  // namespace tm_parse::rules