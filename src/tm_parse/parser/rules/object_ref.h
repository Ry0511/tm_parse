//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser_rule.h"

namespace tm_parse::rules {

class DotIdentifier;

class ObjectRef : public ParserRule {
   private:
    std::unique_ptr<DotIdentifier> m_MainObject{};  // Required
    std::unique_ptr<DotIdentifier> m_SubObject{};   // Optional

   public:
    ObjectRef() = default;
    ~ObjectRef() override = default;

   public:
    ObjectRef(ObjectRef&&) = default;
    ObjectRef& operator=(ObjectRef&&) = default;

   public:
    str rule_name() const noexcept override { return "ObjectRef"; }
    void visit(const std::function<bool(const ParserRule&)>& func) const noexcept override;

    RULE_STATIC_API(ObjectRef);
};

}  // namespace tm_parse::rules