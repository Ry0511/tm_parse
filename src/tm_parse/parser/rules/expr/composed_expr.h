//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse::rules {

// TODO: Probably a good idea to scrap the Expr base class as it just makes things harder to work
//  with.

// TODO: At some point we will want to evaluate expressions, not just composed expressions but all
//  expressions. This evaluation needs to know of things we just don't have access to i.e., the
//  Type of the expression. Some rules have a trivial type deduction i.e., LiteralExpr, but others
//  i.e., VariableExpr and MetaVarExpr don't neccesarily have trivial type deduction.

enum class Operator : uint8_t {
    Add,            // +    Binary
    Subtract,       // -    Binary
    Divide,         // /    Binary
    Multiply,       // *    Binary
    Negate,         // -A   Unary
    Positive,       // +A   Unary; Omitted from parse tree
    LogicalNegate,  // !A   Unary
    LogicalOr,      // or   Binary
    LogicalAnd,     // and  Binary
    Unknown
};

class ComposedExpr;

class UnaryOpExpr : public ParserRule {
   private:
    friend ComposedExpr;
    Operator m_Operator{Operator::Unknown};
    std::unique_ptr<ParserRule> m_Operand;

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
    const ParserRule& operand() const noexcept { return *m_Operand; };

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_CONSTANTS(UnaryOpExpr);
};

class BinaryOpExpr : public ParserRule {
   private:
    friend ComposedExpr;
    Operator m_Operator{};
    std::unique_ptr<ParserRule> m_Left;
    std::unique_ptr<ParserRule> m_Right;

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
    const ParserRule& left() const noexcept { return *m_Left; }
    const ParserRule& right() const noexcept { return *m_Right; }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_CONSTANTS(BinaryOpExpr);
};

class ComposedExpr : public ParserRule {
   private:
    std::unique_ptr<ParserRule> m_Expr;

   public:
    ComposedExpr() noexcept;
    ~ComposedExpr() noexcept override;

   public:
    ComposedExpr(const ComposedExpr&) = delete;
    ComposedExpr& operator=(const ComposedExpr&) = delete;
    ComposedExpr(ComposedExpr&&) noexcept;
    ComposedExpr& operator=(ComposedExpr&&) noexcept;

   public:
    const ParserRule& expr() const noexcept { return *m_Expr; }

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;

   public:
    RULE_STATIC_API(ComposedExpr);

   private:
    static std::unique_ptr<ParserRule> parse_expr(Parser& parser);
    static std::unique_ptr<ParserRule> parse_term(Parser& parser);
    static std::unique_ptr<ParserRule> parse_factor(Parser& parser);
};

}  // namespace tm_parse::rules
