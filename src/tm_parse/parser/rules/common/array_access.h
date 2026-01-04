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
    size_t Index{invalid_index_v};  // Index Value
    bool IsDynamicAccess{false};    // Dynamic=(T) Static=[T]
    bool IsValidNumber{false};      // Was the value of \d valid?
    bool IsInitialised{false};      // Have we been initialised?
};

class ArrayAccess : public ParserRule {
   public:
    /**
     * Going on an assumption that we will never need multi-dimensional array access and if we do
     * it will not exceed this value.
     */
    static constexpr size_t max_array_indexes = 4;

   private:
    std::array<ArrayAccessData, max_array_indexes> m_Data{};

   public:
    ArrayAccess() = default;
    ~ArrayAccess() override = default;

   public:
    ArrayAccess(const ArrayAccess&) = default;
    ArrayAccess& operator=(const ArrayAccess&) = default;
    ArrayAccess(ArrayAccess&&) = default;
    ArrayAccess& operator=(ArrayAccess&&) = default;

   public:
    const auto& array_parts() const noexcept { return m_Data; }
    auto size() const noexcept {
        return std::count_if(m_Data.begin(), m_Data.end(), [](const ArrayAccessData& data) {
            return data.IsInitialised;
        });
    }

   public:
    RULE_STATIC_API(ArrayAccess);
};

}  // namespace tm_parse::rules