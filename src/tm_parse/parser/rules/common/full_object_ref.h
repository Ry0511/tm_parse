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

class FullObjectRef : public ParserRule {
   private:
    std::unique_ptr<ObjectDotIdentifier> m_MainObject;  // Required
    std::unique_ptr<ObjectDotIdentifier> m_SubObject;   // Optional

   public:
    FullObjectRef() noexcept;
    ~FullObjectRef() noexcept override;

   public:
    FullObjectRef(const FullObjectRef&) = delete;
    FullObjectRef& operator=(const FullObjectRef&) = delete;
    FullObjectRef(FullObjectRef&&) noexcept;
    FullObjectRef& operator=(FullObjectRef&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(FullObjectRef);
};

}  // namespace tm_parse::rules