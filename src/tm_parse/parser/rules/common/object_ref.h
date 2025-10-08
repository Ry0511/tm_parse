//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class DotIdentifier;

class ObjectRef : public ParserRule {
   private:
    std::unique_ptr<DotIdentifier> m_MainObject;  // Required
    std::unique_ptr<DotIdentifier> m_SubObject;   // Optional

   public:
    ObjectRef() = default;
    ~ObjectRef() override;

   public:
    ObjectRef(const ObjectRef&) = delete;
    ObjectRef& operator=(const ObjectRef&) = delete;
    ObjectRef(ObjectRef&&) noexcept;
    ObjectRef& operator=(ObjectRef&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;

    RULE_STATIC_API(ObjectRef);
};

}  // namespace tm_parse::rules