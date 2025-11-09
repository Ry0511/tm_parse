//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

// TODO: Probably a good idea to scrap the Expr base class as it just makes things harder to work
// with.

enum class Operator : uint8_t {
    Add,            // +    Binary
    Subtract,       // -    Binary
    Divide,         // /    Binary
    Multiply,       // *    Binary
    Negate,         // -A   Unary
    Positive,       // +A   Unary
    LogicalNegate,  // !A   Unary
    LogicalOr,      // or   Binary
    LogicalAnd,     // and  Binary
    Unknown
};

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
    Operator op() const noexcept { return m_Operator; };
    const Expr& operand() const noexcept { return *m_Operand; };

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_CONSTANTS(UnaryOpExpr);
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
    Operator op() const noexcept { return m_Operator; }
    const Expr& left() const noexcept { return *m_Left; }
    const Expr& right() const noexcept { return *m_Right; }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_CONSTANTS(BinaryOpExpr);
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
    const Expr& expr() const noexcept { return *m_Expr; }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(ComposedExpr);

   private:
    static std::unique_ptr<Expr> parse_expr(Parser& parser);
    static std::unique_ptr<Expr> parse_term(Parser& parser);
    static std::unique_ptr<Expr> parse_factor(Parser& parser);
};

}  // namespace tm_parse::rules
