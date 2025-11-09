//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/composed_expr.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/rules/expr/identifier_ref_expr.h"
#include "tm_parse/parser/rules/expr/literal_expr.h"
#include "tm_parse/parser/rules/expr/meta_var_expr.h"

namespace tm_parse::rules {

////////////////////////////////////////////////////////////////////////////////
// | DEFAULTED FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

UnaryOpExpr::UnaryOpExpr() noexcept = default;
UnaryOpExpr::~UnaryOpExpr() noexcept = default;
UnaryOpExpr::UnaryOpExpr(UnaryOpExpr&&) noexcept = default;
UnaryOpExpr& UnaryOpExpr::operator=(UnaryOpExpr&&) noexcept = default;

BinaryOpExpr::BinaryOpExpr() = default;
BinaryOpExpr::~BinaryOpExpr() = default;
BinaryOpExpr::BinaryOpExpr(BinaryOpExpr&&) noexcept = default;
BinaryOpExpr& BinaryOpExpr::operator=(BinaryOpExpr&&) noexcept = default;

ComposedExpr::ComposedExpr() noexcept = default;
ComposedExpr::~ComposedExpr() noexcept = default;
ComposedExpr::ComposedExpr(ComposedExpr&&) noexcept = default;
ComposedExpr& ComposedExpr::operator=(ComposedExpr&&) noexcept = default;

////////////////////////////////////////////////////////////////////////////////
// | VISITORS |
////////////////////////////////////////////////////////////////////////////////

void UnaryOpExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Operand->visit(func);
}

void BinaryOpExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Left->visit(func);
    m_Right->visit(func);
}

void ComposedExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    Expr::visit(func);
    m_Expr->visit(func);
}

void UnaryOpExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Operand->cascade_assign_parents(this);
}

void BinaryOpExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Left->cascade_assign_parents(this);
    m_Right->cascade_assign_parents(this);
}

void ComposedExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    Expr::cascade_assign_parents(parent);
    m_Expr->cascade_assign_parents(this);
}

////////////////////////////////////////////////////////////////////////////////
// | IMPLEMENTATION |
////////////////////////////////////////////////////////////////////////////////

//
// Broadly following: https://en.wikipedia.org/wiki/Recursive_descent_parser#Example_parser
// extended to support unary operations i.e., -( A + B ) and -( -A + +B ) albeit +B is just for
// symmetry, it does nothing ( omitted from the parsed result as well ).
//
//   i.e, for C++ ( and presumably every other language )
//     int a = -10;
//     int b = +a;  // -10
//     int c = -a;  // 10
//
// see: extended_text_mods.g4 as a simpler outline of how the rules should be processed. But do
// note that this implementation will produce a different tree as it also cleans the emitted
// results.
//

namespace {

// TODO: Change logical negation ! to the keyword not to keep things the same with the other
//  logical operators i.e., and, or

constinit tk::TokenKind op_unary_operators[]{tk::Plus, tk::Minus, tk::ExclamationMark};
constinit tk::TokenKind op_high_precedence[]{tk::Star, tk::Slash, tk::And};
constinit tk::TokenKind op_low_precedence[]{tk::Plus, tk::Minus, tk::Or};

Operator get_unary_op_kind(const Token& tok) noexcept {
    // clang-format off
    switch (tok.Kind) {
        case tk::Plus:            return Operator::Positive;
        case tk::Minus:           return Operator::Negate;
        case tk::ExclamationMark: return Operator::LogicalNegate; // TODO: Might as well make this an identifier
    }
    // clang-format on
    return Operator::Unknown;
}

Operator get_binary_op_kind(const Token& tok) noexcept {
    // clang-format off
    switch (tok.Kind) {
        case tk::Plus:  return Operator::Add;
        case tk::Minus: return Operator::Subtract;
        case tk::Star:  return Operator::Multiply;
        case tk::Slash: return Operator::Divide;
        case tk::And:   return Operator::LogicalAnd;
        case tk::Or:    return Operator::LogicalOr;
    }
    // clang-format on
    return Operator::Unknown;
}

bool match_expr(Matcher& matcher);
bool match_term(Matcher& matcher);
bool match_factor(Matcher& matcher);

bool match_expr(Matcher& matcher) {
    match_term(matcher);

    while (matcher.any_real(op_low_precedence)) {
        if (!match_term(matcher)) {
            return false;
        }
    }

    return true;
}

bool match_term(Matcher& matcher) {
    if (!match_factor(matcher)) {
        return false;
    }

    while (matcher.any_real(op_high_precedence)) {
        if (!match_factor(matcher)) {
            return false;
        }
    }

    return true;
}

bool match_factor(Matcher& matcher) {
    if (LiteralExpr::matches(matcher) || IdentifierRefExpr::matches(matcher)
        || MetaVarExpr::matches(matcher)) {
        return true;
    }

    // sub expression
    if (matcher.maybe_real(tk::LeftParen)) {
        if (!match_expr(matcher)) {
            return false;
        }
        return matcher.maybe_real(tk::RightParen);
    }

    return matcher.any_real(op_unary_operators);
}

}  // namespace

bool ComposedExpr::matches(Matcher& matcher) noexcept {
    return match_expr(matcher);
}

std::unique_ptr<ComposedExpr> ComposedExpr::create(Parser& parser) {
    auto rule = std::make_unique<ComposedExpr>();
    rule->m_Expr = parse_expr(parser);
    rule->copy_state(*rule->m_Expr);
    return rule;
}

std::unique_ptr<Expr> ComposedExpr::parse_expr(Parser& parser) {
    std::unique_ptr<Expr> node = parse_term(parser);

    while (Token cur = parser.any_real(op_low_precedence)) {
        Operator op = get_binary_op_kind(cur);

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = parse_term(parser);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);

        node = std::move(binary_op);
    }

    return node;
}

std::unique_ptr<Expr> ComposedExpr::parse_term(Parser& parser) {
    auto node = parse_factor(parser);
    Matcher m = parser.create_matcher();

    while (Token cur = m.any_real(op_high_precedence)) {
        Operator op = get_binary_op_kind(cur);
        parser.set_position(m.position());

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = parse_factor(parser);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);

        node = std::move(binary_op);
        m.set_position(parser.position());
    }

    return node;
}

std::unique_ptr<Expr> ComposedExpr::parse_factor(Parser& parser) {
    // simple literal
    Matcher matcher = parser.create_matcher();

    if (matcher.matches<LiteralExpr>()) {
        return LiteralExpr::create(parser);
    }

    // simple variable
    if (matcher.matches<IdentifierRefExpr>()) {
        return IdentifierRefExpr::create(parser);
    }

    // meta var via $(foo.baz.bar)
    if (matcher.matches<MetaVarExpr>()) {
        return MetaVarExpr::create(parser);
    }

    // sub expression
    if (Token first = parser.maybe_real(tk::LeftParen)) {
        auto node = parse_expr(parser);
        Token last = parser.require_real(tk::RightParen);
        node->post_init(first, last);
        return node;
    }

    // unary operator + is generally ignored/no op and only done for symmetry, its only usage/change
    // is that the rules full text region will include it
    if (Token first = parser.maybe_real(tk::Plus)) {
        auto node = parse_factor(parser);
        node->post_init(first, node->last_token());
        return node;
    }

    // negation is just sugar that allows for writing -A instead of (A * -1); Can also apply to
    //  groups i.e., -(A * B)
    constexpr tk::TokenKind unary_operator_tokens[]{tk::Minus, tk::ExclamationMark};
    if (Token first = parser.any_real(unary_operator_tokens)) {
        auto unary = std::make_unique<UnaryOpExpr>();
        unary->m_Operator = get_unary_op_kind(first);
        unary->m_Operand = parse_factor(parser);
        unary->post_init(first, unary->m_Operand->last_token());
        return unary;
    }

    // Don't know what this is
    Token cur = matcher.next_real();
    throw TokenError{"unexpected input in composed expression", cur};
}

}  // namespace tm_parse::rules
