//
// Date       : 30/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

struct ArrayAccessData {
    size_t Index{0};
    bool IsDynamicAccess{false};
    bool IsValidNumber{false};
};

class ArrayAccess : public ParserRule {
   private:
    std::vector<ArrayAccessData> m_Data;

   public:
    ArrayAccess() = default;
    ~ArrayAccess() override;

   public:
    ArrayAccess(const ArrayAccess&) = default;
    ArrayAccess& operator=(const ArrayAccess&) = default;
    ArrayAccess(ArrayAccess&&) = default;
    ArrayAccess& operator=(ArrayAccess&&) = default;

   public:
    const std::vector<ArrayAccessData>& array_access_data() const noexcept { return m_Data; }
    const ArrayAccessData& first() const noexcept { return m_Data.front(); }
    const ArrayAccessData& last() const noexcept { return m_Data.back(); }
    size_t size() const noexcept { return m_Data.size(); }

    auto begin() const noexcept { return m_Data.begin(); }
    auto end() const noexcept { return m_Data.end(); }

   public:
    RULE_STATIC_API(ArrayAccess);
};

}  // namespace tm_parse::rules