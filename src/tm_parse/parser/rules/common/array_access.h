//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

class ArrayAccess : public ParserRule {
   private:
    int64_t m_Index{0};
    bool m_IsDynamicAccess{false};
    bool m_IsValidNumber{false};

   public:
    ArrayAccess() = default;
    ~ArrayAccess() override;

   public:
    ArrayAccess(const ArrayAccess&) = default;
    ArrayAccess& operator=(const ArrayAccess&) = default;
    ArrayAccess(ArrayAccess&&) = default;
    ArrayAccess& operator=(ArrayAccess&&) = default;

   public:
    RULE_STATIC_API(ArrayAccess);
};

}  // namespace tm_parse::rules