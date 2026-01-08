//
// Date       : 08/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "variant"
#include "cstdint"

namespace tm_parse {

struct Number {
   public:
    using Inner = std::variant<std::int64_t, double>;

   private:
    Inner m_Value;

    // clang-format off
   public:
    constexpr Number(std::int32_t value) : m_Value(static_cast<std::int64_t>(value)) {}
    constexpr Number(std::int64_t value) : m_Value(value) {}
    constexpr Number(float value) : m_Value(static_cast<double>(value)) {}
    constexpr Number(double value) : m_Value(value) {}
    // clang-format on

    constexpr bool is_integer() const noexcept { return std::holds_alternative<std::int64_t>(m_Value); }
    constexpr bool is_float() const noexcept { return std::holds_alternative<double>(m_Value); }

   public:
    constexpr auto value() const noexcept { return m_Value; }
    constexpr auto as_int() const noexcept {
        return std::visit(
            [](const auto& val) { return static_cast<std::int64_t>(val); },
            m_Value
        );
    }
    constexpr auto as_float() const noexcept {
        return std::visit(
            [](const auto& val) { return static_cast<double>(val); },
            m_Value
        );
    }

   public:
    constexpr Number operator+(const Number& rhs) const noexcept {
        return std::visit(
            [](auto left, auto right) { return Number{left + right}; },
            m_Value,
            rhs.m_Value
        );
    }

    constexpr Number operator-(const Number& rhs) const noexcept {
        return std::visit(
            [](auto left, auto right) { return Number{left - right}; },
            m_Value,
            rhs.m_Value
        );
    }

    constexpr Number operator/(const Number& rhs) const noexcept {
        return std::visit(
            [](auto left, auto right) { return Number{left / right}; },
            m_Value,
            rhs.m_Value
        );
    }

    constexpr Number operator*(const Number& rhs) const noexcept {
        return std::visit(
            [](auto left, auto right) { return Number{left * right}; },
            m_Value,
            rhs.m_Value
        );
    }

    constexpr Number operator-() const noexcept {
        return std::visit([](auto value) { return Number{-value}; }, m_Value);
    }
};

}  // namespace tm_parse