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
//   So we have
//     Expression ::= expression = ["+"|"-"] term {("+"|"-") term} .
//     Term       ::= term = factor {("*"|"/") factor} .
//     Factor     ::= ident | number | "(" expression ")" .
//
// Note
//   - [ X ]  means X is optional
//   -  "X"   means X must appear exactly as is
//   - { X }  means X is a repeating sequence similar to X* (zero or more)
//   - ( X )  is a generic grouping
//   -  X|Y   means one of X or Y
//
// So: expression = ["+"|"-"] term {("+"|"-") term} .
//   Is: [+-]? term ([+-] term)*
//

namespace {

bool match_expr(Matcher& matcher);
bool match_term(Matcher& matcher);
bool match_factor(Matcher& matcher);

bool match_expr(Matcher& matcher) {
    bool has_unary = matcher.maybe_real(tk::Plus) || matcher.maybe_real(tk::Minus);
    (void)has_unary;

    match_term(matcher);

    constexpr tk::TokenKind valid_tokens[]{tk::Plus, tk::Minus};

    while (matcher.any_real(valid_tokens)) {
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

    constexpr tk::TokenKind valid_tokens[]{tk::Star, tk::Slash};

    while (matcher.any_real(valid_tokens)) {
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
        return matcher.require_real(tk::RightParen);
    }

    return false;
}

}  // namespace

bool ComposedExpr::matches(Matcher& matcher) noexcept {
    return match_expr(matcher);
}

std::unique_ptr<ComposedExpr> ComposedExpr::create(Parser& parser) {
    return parse_expr(parser);
}

// TODO: The child nodes need to have their parent post initialised ( also setting the parents as
// well )

std::unique_ptr<ComposedExpr> ComposedExpr::parse_expr(Parser& parser) {
    Operator unary_op = Operator::Unknown;

    Token first = parser.maybe_next_real(tk::Plus);

    if (first) {
        unary_op = Operator::Positive;
    } else {
        first = parser.maybe_next_real(tk::Minus);
        if (first) {
            unary_op = Operator::Negate;
        }
    }

    std::unique_ptr<Expr> node = parse_term(parser);

    if (unary_op == Operator::Positive || unary_op == Operator::Negate) {
        auto unary = std::make_unique<UnaryOpExpr>();
        unary->m_Operator = unary_op;
        unary->m_Operand = std::move(node);
        unary->post_init(first, unary->m_Operand->last_token());
        node = std::move(unary);
    }

    constexpr tk::TokenKind valid_tokens[]{tk::Plus, tk::Minus};
    Matcher m = parser;

    while (Token cur = m.any_real(valid_tokens)) {
        Operator op = (cur == tk::Plus) ? Operator::Add : Operator::Subtract;

        parser.set_position(m.position());
        std::unique_ptr<Expr> right = parse_term(parser);
        m.set_position(parser.position());

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = std::move(right);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);

        node = std::move(binary_op);
    }

    auto rule = std::make_unique<ComposedExpr>();
    rule->m_Expr = std::move(node);
    rule->copy_state(*rule->m_Expr);
    return rule;
}

std::unique_ptr<Expr> ComposedExpr::parse_term(Parser& parser) {
    auto node = parse_factor(parser);

    constexpr tk::TokenKind valid_tokens[]{tk::Star, tk::Slash};
    Matcher m = parser;

    while (Token cur = m.any_real(valid_tokens)) {
        Operator op = (cur == tk::Star) ? Operator::Multiply : Operator::Divide;
        parser.set_position(m.position());
        auto right = parse_factor(parser);
        m.set_position(parser.position());

        auto binary_op = std::make_unique<BinaryOpExpr>();
        binary_op->m_Operator = op;
        binary_op->m_Left = std::move(node);
        binary_op->m_Right = std::move(right);
        binary_op->post_init(*binary_op->m_Left, *binary_op->m_Right);
        node = std::move(binary_op);
    }

    return node;
}

std::unique_ptr<Expr> ComposedExpr::parse_factor(Parser& parser) {
    // simple literal
    Matcher matcher = parser;
    if (LiteralExpr::matches(matcher)) {
        return LiteralExpr::create(parser);
    }

    // simple variable
    matcher = parser;
    if (IdentifierRefExpr::matches(matcher)) {
        return IdentifierRefExpr::create(parser);
    }

    // meta var via $(foo.baz.bar)
    matcher = parser;
    if (MetaVarExpr::matches(matcher)) {
        return MetaVarExpr::create(parser);
    }

    // sub expression
    matcher = parser;
    if (Token first = parser.maybe_next_real(tk::LeftParen)) {
        auto node = parse_expr(parser);
        Token last = parser.require_next_real(tk::RightParen);
        node->post_init(first, last);
        return node;
    }

    if (Token first = parser.maybe_next_real(tk::Plus)) {
        auto rule = parse_expr(parser);
        rule->post_init(first, rule->last_token());
        return rule;
    }

    if (Token first = parser.maybe_next_real(tk::Minus)) {
        auto rule = parse_expr(parser);
        rule->post_init(first, rule->last_token());
        return rule;
    }

    if (parser.maybe_next_real(tk::Minus) || parser.maybe_next_real(tk::Plus)) {
        return parse_expr(parser);
    }

    // Don't know what this is
    Token cur = parser.next_real();
    throw TokenError{
        "expecting one of [LiteralExpr, IdentifierRefExpr, MetaVarExpr, or ComposedExpr]",
        cur
    };
}

}  // namespace tm_parse::rules
