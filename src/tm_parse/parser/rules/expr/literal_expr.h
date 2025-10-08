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
    using ValueType = std::variant<std::monostate, int64_t, double, bool, str>;
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
    template <class T>
    bool is() const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return std::holds_alternative<double>(m_Value);
        }
        // char, short, int, etc
        else if constexpr (std::is_integral_v<T>) {
            return std::holds_alternative<int64_t>(m_Value);
        }
        // assume str
        else {
            static_assert(std::is_same_v<T, str>);
            return std::holds_alternative<T>(m_Value);
        }
    }

    bool has_value() const noexcept { return std::holds_alternative<std::monostate>(m_Value); }

    template <class T>
    const T& get() const noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            return static_cast<T>(std::get<double>(m_Value));
        }
        // char, short, int, etc
        else if constexpr (std::is_integral_v<T>) {
            return static_cast<T>(std::get<int64_t>(m_Value));
        }
        // assume str
        else {
            static_assert(std::is_same_v<T, str>);
            return std::get<T>(m_Value);
        }
    }

   public:
    RULE_STATIC_API(LiteralExpr);
};

}  // namespace tm_parse::rules
