//
// Date       : 05/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/primary/object_definition.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/obj_dot_identifier.h"
#include "tm_parse/parser/rules/expr/assignment_expr.h"
#include "tm_parse/parser/rules/pragmas/pragma_toggle.h"

namespace tm_parse::rules {

ObjectDefinition::ObjectDefinition() noexcept = default;
ObjectDefinition::~ObjectDefinition() noexcept = default;
ObjectDefinition::ObjectDefinition(ObjectDefinition&&) noexcept = default;
ObjectDefinition& ObjectDefinition::operator=(ObjectDefinition&&) noexcept = default;

bool ObjectDefinition::matches(Matcher& m) noexcept {
    // Name = ? Class = ?
    if (m.match_real_seq<tk::Begin, tk::Object, tk::Name, tk::Equal>()) {
        if (
            !(m.matches<ObjectDotIdentifier>()
              && m.match_real_seq<tk::Class, tk::Equal>()
              && m.matches<ObjectDotIdentifier>())
        ) {
            return false;
        }
    }
    // Class = ? Name = ?
    else if (m.match_real_seq<tk::Begin, tk::Object, tk::Class, tk::Equal>()) {
        if (
            !(m.matches<ObjectDotIdentifier>()
              && m.match_real_seq<tk::Name, tk::Equal>()
              && m.matches<ObjectDotIdentifier>())
        ) {
            return false;
        }
    }
    // no header matches
    else {
        return false;
    }

    while (!m.match_real_seq<tk::End, tk::Object>()) {
        if (!m.matches_one_of<PragmaToggle, ObjectDefinition, AssignmentExpr>()) {
            return false;
        }
    }

    return true;
}

std::unique_ptr<ObjectDefinition> ObjectDefinition::create(Parser& parser) {
    const Token& first = parser.require_real(tk::Begin);
    parser.require_real(tk::Object);

    auto rule = std::make_unique<ObjectDefinition>();

    if (parser.maybe_real(tk::Name)) {
        parser.require_real(tk::Equal);
        rule->m_ObjectName = parser.create<ObjectDotIdentifier>();
        parser.require_real(tk::Class);
        parser.require_real(tk::Equal);
        rule->m_ClassName = parser.create<ObjectDotIdentifier>();

    } else {
        parser.require_real(tk::Class);
        parser.require_real(tk::Equal);
        rule->m_ClassName = parser.create<ObjectDotIdentifier>();
        parser.require_real(tk::Name);
        parser.require_real(tk::Equal);
        rule->m_ObjectName = parser.create<ObjectDotIdentifier>();
    }

    rule->m_ClassName->set_parent(*rule);
    rule->m_ObjectName->set_parent(*rule);

    while (!parser.match_real_seq<tk::End, tk::Object>()) {
        size_t pos = parser.position();

        // Do a quick evaluation check instead of a full match check here
        if (parser.match_real_seq<tk::Begin, tk::Object, tk::AnyIdentifier>()) {
            parser.set_position(pos);
            auto& r = rule->m_ChildRules.emplace_back(parser.create<ObjectDefinition>());
            r->set_parent(*rule);
        }
        // trivial to do a full check
        else {
            parser.set_position(pos);
            auto& r = rule->m_ChildRules.emplace_back(
                parser.create_one_of<AssignmentExpr, PragmaToggle>()
            );
            r->set_parent(*rule);
        }
    }

    // peek should be tk::Object
    rule->post_init(first, parser.peek());

    return rule;
}

void ObjectDefinition::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_ClassName->visit(func);
    m_ObjectName->visit(func);
    for (const auto& child : m_ChildRules) {
        child->visit(func);
    }
}

void ObjectDefinition::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_ClassName->cascade_assign_parents(this);
    m_ObjectName->cascade_assign_parents(this);
    for (const auto& child : m_ChildRules) {
        child->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
