//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class PropertyDotIdentifier;
class FullObjectRef;

// foo.baz.bar'bar.baz:foo'
class ClassObjectRef : public ParserRule {
   private:
    std::unique_ptr<PropertyDotIdentifier> m_Class;
    std::unique_ptr<FullObjectRef> m_Object;

   public:
    ClassObjectRef();
    ~ClassObjectRef() override;

   public:
    ClassObjectRef(const ClassObjectRef&) = delete;
    ClassObjectRef& operator=(const ClassObjectRef&) = delete;
    ClassObjectRef(ClassObjectRef&&) noexcept;
    ClassObjectRef& operator=(ClassObjectRef&&) noexcept;

   public:
    const PropertyDotIdentifier& clazz() const;
    const FullObjectRef& object() const;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(ClassObjectRef);
};

}  // namespace tm_parse::rules
