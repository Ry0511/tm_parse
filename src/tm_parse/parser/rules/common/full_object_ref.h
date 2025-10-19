//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class DotIdentifier;

class FullObjectRef : public ParserRule {
   private:
    std::unique_ptr<DotIdentifier> m_MainObject;  // Required
    std::unique_ptr<DotIdentifier> m_SubObject;   // Optional

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
    RULE_STATIC_API(FullObjectRef);
};

}  // namespace tm_parse::rules