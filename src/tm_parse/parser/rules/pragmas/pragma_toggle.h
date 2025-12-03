//
// Date       : 02/12/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

enum class PragmaToggleType : uint8_t {
    UnquotedLiterals
};

class PragmaToggle : public ParserRule {
   private:
    PragmaToggleType m_Type;

   public:
    PragmaToggle() noexcept = default;
    ~PragmaToggle() noexcept override = default;

    PragmaToggle(const PragmaToggle&) = default;
    PragmaToggle& operator=(const PragmaToggle&) = default;
    PragmaToggle(PragmaToggle&&) = default;
    PragmaToggle& operator=(PragmaToggle&&) = default;

   public:
    void toggle_for(Parser& parser) const noexcept;

   public:
    RULE_STATIC_API(PragmaToggle);
};

}  // namespace tm_parse::rules
