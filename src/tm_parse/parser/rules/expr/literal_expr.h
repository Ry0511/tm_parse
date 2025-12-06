//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class LiteralExpr : public Expr {
   private:
    // Kinda torn here on if we should default std::nullopt to std::monostate
    using ValueType = std::variant<std::monostate, NoneType, Float, Int, Bool, Str>;
    ValueType m_Value;

   public:
    LiteralExpr() = default;
    ~LiteralExpr() override;

   public:
    LiteralExpr(const LiteralExpr&) = default;
    LiteralExpr& operator=(const LiteralExpr&) = default;
    LiteralExpr(LiteralExpr&&) noexcept = default;
    LiteralExpr& operator=(LiteralExpr&&) noexcept = default;

   public:
    bool has_value() const noexcept { return !std::holds_alternative<std::monostate>(m_Value); }

    template <class T>
    const T* get_if() const {
        return std::get_if<T>(&m_Value);
    }

   public:
    RULE_STATIC_API(LiteralExpr);
};

}  // namespace tm_parse::rules
