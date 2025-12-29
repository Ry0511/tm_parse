//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class ObjectDotIdentifier;

class ClassObjectRef : public ParserRule {
   private:
    str m_Class;
    std::unique_ptr<ObjectDotIdentifier> m_Object;

   public:
    ClassObjectRef();
    ~ClassObjectRef() override;

   public:
    ClassObjectRef(const ClassObjectRef&) = delete;
    ClassObjectRef& operator=(const ClassObjectRef&) = delete;
    ClassObjectRef(ClassObjectRef&&) noexcept;
    ClassObjectRef& operator=(ClassObjectRef&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    RULE_STATIC_API(ClassObjectRef);
};

}  // namespace tm_parse::rules
