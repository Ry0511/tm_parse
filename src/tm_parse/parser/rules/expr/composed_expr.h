//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

enum class Operator : uint8_t { Add, Subtract, Divide, Multiply, Negate, Positive, Unknown };

class ComposedExpr;

class UnaryOpExpr : public Expr {
   private:
    friend ComposedExpr;
    Operator m_Operator{Operator::Unknown};
    std::unique_ptr<Expr> m_Operand;

   public:
    UnaryOpExpr() noexcept;
    ~UnaryOpExpr() noexcept override;

   public:
    UnaryOpExpr(const UnaryOpExpr&) = delete;
    UnaryOpExpr& operator=(const UnaryOpExpr&) = delete;
    UnaryOpExpr(UnaryOpExpr&&) noexcept;
    UnaryOpExpr& operator=(UnaryOpExpr&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    str rule_name() const noexcept override { return "UnaryOpExpr"; }
};

class BinaryOpExpr : public Expr {
   private:
    friend ComposedExpr;
    Operator m_Operator{};
    std::unique_ptr<Expr> m_Left;
    std::unique_ptr<Expr> m_Right;

   public:
    BinaryOpExpr();
    ~BinaryOpExpr() override;

   public:
    BinaryOpExpr(const BinaryOpExpr&) = delete;
    BinaryOpExpr& operator=(const BinaryOpExpr&) = delete;
    BinaryOpExpr(BinaryOpExpr&&) noexcept;
    BinaryOpExpr& operator=(BinaryOpExpr&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    str rule_name() const noexcept override { return "BinaryOpExpr"; }
};

class ComposedExpr : public Expr {
   private:
    std::unique_ptr<Expr> m_Expr;

   public:
    ComposedExpr() noexcept;
    ~ComposedExpr() noexcept override;

   public:
    ComposedExpr(const ComposedExpr&) = delete;
    ComposedExpr& operator=(const ComposedExpr&) = delete;
    ComposedExpr(ComposedExpr&&) noexcept;
    ComposedExpr& operator=(ComposedExpr&&) noexcept;

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
    RULE_STATIC_API(ComposedExpr);

   private:
    static std::unique_ptr<ComposedExpr> parse_expr(Parser& parser);
    static std::unique_ptr<Expr> parse_term(Parser& parser);
    static std::unique_ptr<Expr> parse_factor(Parser& parser);
};

}  // namespace tm_parse::rules
