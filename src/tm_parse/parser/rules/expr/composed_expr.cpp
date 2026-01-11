//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/composed_expr.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"
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
// | METHODS |
////////////////////////////////////////////////////////////////////////////////

void UnaryOpExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Operand->visit(func);
}

void BinaryOpExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Left->visit(func);
    m_Right->visit(func);
}

void ComposedExpr::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Expr->visit(func);
}

void UnaryOpExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Operand->cascade_assign_parents(this);
}

void BinaryOpExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Left->cascade_assign_parents(this);
    m_Right->cascade_assign_parents(this);
}

void ComposedExpr::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Expr->cascade_assign_parents(this);
}

void UnaryOpExpr::simplify_ast() noexcept {
    m_Operand->simplify_ast();
    std::optional<Number> res = m_Operand->evaluate_numeric_expr();
    if (res.has_value()) {
        m_Operand = std::make_unique<LiteralExpr>(*m_Operand, res->value());
        m_Operand->set_parent(*this);
    }
}

void BinaryOpExpr::simplify_ast() noexcept {
    m_Left->simplify_ast();
    m_Right->simplify_ast();

    auto left = m_Left->evaluate_numeric_expr();
    if (left.has_value()) {
        m_Left = std::make_unique<LiteralExpr>(*m_Left, left->value());
        m_Left->set_parent(*this);
    }

    auto right = m_Right->evaluate_numeric_expr();
    if (right.has_value()) {
        m_Right = std::make_unique<LiteralExpr>(*m_Right, right->value());
        m_Right->set_parent(*this);
    }
}

void ComposedExpr::simplify_ast() noexcept {
    m_Expr->simplify_ast();
    auto res = m_Expr->evaluate_numeric_expr();
    if (res.has_value()) {
        m_Expr = std::make_unique<LiteralExpr>(*m_Expr, res->value());
        m_Expr->set_parent(*this);
    }
}

std::optional<Number> UnaryOpExpr::evaluate_numeric_expr() noexcept {
    auto res = m_Operand->evaluate_numeric_expr();
    if (res.has_value() && m_Operator == NumericOperator::Negate) {
        return -(*res);
    }
    return res;
}

std::optional<Number> BinaryOpExpr::evaluate_numeric_expr() noexcept {
    auto left = m_Left->evaluate_numeric_expr();
    auto right = m_Right->evaluate_numeric_expr();

    if (!left.has_value() || !right.has_value()) {
        return std::nullopt;
    }

    // clang-format off
    switch (m_Operator) {
        case NumericOperator::Add     : return (*left) + (*right);
        case NumericOperator::Subtract: return (*left) - (*right);
        case NumericOperator::Divide  : return (*left) / (*right);
        case NumericOperator::Multiply: return (*left) * (*right);
        default: break;
    }
    // clang-format on
    return std::nullopt;
}

std::optional<Number> ComposedExpr::evaluate_numeric_expr() noexcept {
    return m_Expr->evaluate_numeric_expr();
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

constexpr std::array<tk::TokenKind, 2> op_unary_operators{tk::Plus, tk::Minus};
constexpr std::array<tk::TokenKind, 2> op_high_precedence{tk::Star, tk::Slash};
constexpr std::array<tk::TokenKind, 2> op_low_precedence{tk::Plus, tk::Minus};
constexpr std::array<tk::TokenKind, 2> unary_operator_tokens{tk::Minus, tk::Plus};

NumericOperator get_unary_op_kind(const Token& tok) {
    // clang-format off
    switch (tok.Kind) {
        case tk::Plus : return NumericOperator::Positive;
        case tk::Minus: return NumericOperator::Negate;
        default:
            throw std::runtime_error{std::format("invalid unary operator token {}", tok.text())};
    }
    // clang-format off
}

NumericOperator get_binary_op_kind(const Token& tok) {
    // clang-format off
    switch (tok.Kind) {
        case tk::Plus : return NumericOperator::Add;
        case tk::Minus: return NumericOperator::Subtract;
        case tk::Star : return NumericOperator::Multiply;
        case tk::Slash: return NumericOperator::Divide;
        default:
            throw std::runtime_error{std::format("invalid binary operator token {}", tok.text())};
    }
    // clang-format off
}

bool match_expr(Matcher& matcher, bool& has_any_op);
bool match_term(Matcher& matcher, bool& has_any_op);
bool match_factor(Matcher& matcher, bool& has_any_op);

bool match_expr(Matcher& matcher, bool& has_any_op) {
    if (!match_term(matcher, has_any_op)) {
        return false;
    }

    while (matcher.any_real(op_low_precedence)) {
        has_any_op = true;
        if (!match_term(matcher, has_any_op)) {
            return false;
        }
    }

    return true;
}

bool match_term(Matcher& matcher, bool& has_any_op) {
    if (!match_factor(matcher, has_any_op)) {
        return false;
    }

    while (matcher.any_real(op_high_precedence)) {
        has_any_op = true;
        if (!match_factor(matcher, has_any_op)) {
            return false;
        }
    }

    return true;
}

bool match_factor(Matcher& matcher, bool& has_any_op) {
    if (
        LiteralExpr::matches(matcher)
        || PropertyDotIdentifier::matches(matcher)
        || MetaVarExpr::matches(matcher)
    ) {
        return true;
    }

    if (matcher.maybe_real(tk::LeftParen)) {
        if (!match_expr(matcher, has_any_op)) {
            return false;
        }
        return matcher.maybe_real(tk::RightParen);
    }

    if (matcher.any_real(op_unary_operators)) {
        has_any_op = true;
        return true;
    }

    return false;
}

}  // namespace

bool ComposedExpr::matches(Matcher& matcher) noexcept {
    // Extra check here to prevent single identifier/number tokens from parsing as composed
    // expressions i.e., "set foo baz 1" should have a LiteralExpr not a ComposedExpr(LiteralExpr)
    // this also allows unquoted string literals to work since those literals have the lowest priority.
    bool has_any_op = false;
    return match_expr(matcher, has_any_op) && has_any_op;
}

std::unique_ptr<ComposedExpr> ComposedExpr::create(Parser& parser) {
    auto rule = std::make_unique<ComposedExpr>();
    rule->m_Expr = parse_expr(parser);
    rule->m_Expr->set_parent(*rule);
    rule->copy_state(*rule->m_Expr);
    return rule;
}

std::unique_ptr<ParserRule> ComposedExpr::parse_expr(Parser& parser) {
    std::unique_ptr<ParserRule> node = parse_term(parser);

    while (const Token& cur = parser.any_real(op_low_precedence)) {
        NumericOperator op = get_binary_op_kind(cur);

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = parse_term(parser);
        binary_op->m_Left->set_parent(*binary_op);
        binary_op->m_Right->set_parent(*binary_op);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);

        node = std::move(binary_op);
    }

    return node;
}

std::unique_ptr<ParserRule> ComposedExpr::parse_term(Parser& parser) {
    auto node = parse_factor(parser);

    while (const Token& cur = parser.any_real(op_high_precedence)) {
        NumericOperator op = get_binary_op_kind(cur);

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = parse_factor(parser);
        binary_op->m_Left->set_parent(*binary_op);
        binary_op->m_Right->set_parent(*binary_op);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);

        node = std::move(binary_op);
    }

    return node;
}

std::unique_ptr<ParserRule> ComposedExpr::parse_factor(Parser& parser) {
    // simple literal
    Matcher matcher = parser.create_matcher();

    // sub expression
    if (const Token& first = parser.maybe_real(tk::LeftParen)) {
        auto node = parse_expr(parser);
        const Token& last = parser.require_real(tk::RightParen);
        node->post_init(first, last);
        return node;
    }

    // unary operator + is generally ignored/no op and only done for symmetry, its only usage/change
    // is that the rules full text region will include it.
    if (const Token& first = parser.maybe_real(tk::Plus)) {
        auto node = parse_factor(parser);
        node->post_init(first, node->last_token());
        return node;
    }

    if (matcher.matches<LiteralExpr>()) {
        return LiteralExpr::create(parser);
    }

    // unary operators
    if (const Token& first = parser.any_real(unary_operator_tokens)) {
        auto unary = std::make_unique<UnaryOpExpr>();
        unary->m_Operator = get_unary_op_kind(first);
        unary->m_Operand = parse_factor(parser);
        unary->m_Operand->set_parent(*unary);
        unary->post_init(first, unary->m_Operand->last_token());
        return unary;
    }

    // simple variable
    if (matcher.matches<PropertyDotIdentifier>()) {
        return PropertyDotIdentifier::create(parser);
    }

    // meta var via $foo.baz.bar
    if (matcher.matches<MetaVarExpr>()) {
        return MetaVarExpr::create(parser);
    }

    // Don't know what this is
    const Token& cur = matcher.next_real();
    throw TokenError{"unexpected input in composed expression", cur};
}

}  // namespace tm_parse::rules
