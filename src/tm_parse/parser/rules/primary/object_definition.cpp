//
// Date       : 05/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/primary/object_definition.h"

#include "tm_parse/parser/rules/expr/assignment_expr.h"

namespace tm_parse::rules {

namespace {

// Unsure if we should enforce ordering of class/name but it simplifies things so might as well
// Begin Object Class=Foo Name=Baz
constexpr tk::TokenKind obj_def_start_seq[]{
    tk::Begin,
    tk::Object,
    tk::Class,
    tk::Equal,
    tk::AnyIdentifier,
    tk::Name,
    tk::Equal,
    tk::AnyIdentifier
};

// A =
constexpr tk::TokenKind prop_write_seq[]{tk::AnyIdentifier, tk::Equal};

// A(0) =
constexpr tk::TokenKind dyn_array_prop_write_seq[]{
    tk::AnyIdentifier,
    tk::LeftParen,
    tk::Number,
    tk::RightParen,
    tk::Equal
};

// A[0] =
constexpr tk::TokenKind static_array_prop_write_seq[]{
    tk::AnyIdentifier,
    tk::LeftBracket,
    tk::Number,
    tk::RightBracket,
    tk::Equal
};

bool is_assignment_seq(Matcher& m) noexcept {
    return m.try_match_real(prop_write_seq) || m.try_match_real(dyn_array_prop_write_seq)
           || m.try_match_real(static_array_prop_write_seq);
}

}  // namespace

ObjectDefinition::~ObjectDefinition() = default;
ObjectDefinition::ObjectDefinition(ObjectDefinition&&) noexcept = default;
ObjectDefinition& ObjectDefinition::operator=(ObjectDefinition&&) noexcept = default;

bool ObjectDefinition::matches(Matcher& matcher) noexcept {
    if (!matcher.try_match_real(obj_def_start_seq)) {
        return false;
    }

    bool exit_found = false;

    while (!exit_found) {
        // only need this to perform a lookahead to see if we can start a nested object or an
        //  assignment expression
        Matcher m{matcher};

        // check for child objects first
        if (m.try_match_real(obj_def_start_seq)) {
            if (!ObjectDefinition::matches(matcher)) {
                return false;
            }
        }
        // check for property assignments if possible
        else if (is_assignment_seq(m)) {
            if (!AssignmentExpr::matches(matcher)) {
                return false;
            }
        }
        // neither assignment nor object definition could be deduced so exit here
        else {
            exit_found = true;
        }
    }

    return matcher.maybe_real(tk::End) && matcher.maybe_real(tk::Object);
}

std::unique_ptr<ObjectDefinition> ObjectDefinition::create(Parser& parser) {
    auto rule = std::make_unique<ObjectDefinition>();

    Token first = parser.require_real(tk::Begin);
    parser.require_real(tk::Object);

    // Class=\w+
    parser.require_real(tk::Class);
    parser.require_real(tk::Equal);
    Token obj_class = parser.require_real(tk::AnyIdentifier);

    // Name=\w+
    parser.require_real(tk::Name);
    parser.require_real(tk::Equal);
    Token obj_name = parser.require_real(tk::AnyIdentifier);

    bool exit_found = false;

    while (!exit_found) {
        Matcher m = parser.create_matcher();

        // child object
        if (m.try_match_real(obj_def_start_seq)) {
            auto& obj = rule->m_ChildObjects.emplace_back(ObjectDefinition::create(parser));
            obj->set_parent(*rule);
        }
        // property write
        else if (is_assignment_seq(m)) {
            auto& assign = rule->m_PropertyWrites.emplace_back(AssignmentExpr::create(parser));
            assign->set_parent(*rule);
        } else {
            exit_found = true;
        }
    }

    parser.require_real(tk::End);
    Token last = parser.require_real(tk::Object);

    rule->post_init(first, last);
    rule->m_ClassName = str{obj_class.text()};
    rule->m_ObjectName = str{obj_name.text()};

    return rule;
}

void ObjectDefinition::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);

    for (const auto& child : m_ChildObjects) {
        child->visit(func);
    }

    for (const auto& property : m_PropertyWrites) {
        property->visit(func);
    }
}

void ObjectDefinition::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);

    for (const auto& child : m_ChildObjects) {
        child->cascade_assign_parents(this);
    }

    for (const auto& assign : m_PropertyWrites) {
        assign->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
