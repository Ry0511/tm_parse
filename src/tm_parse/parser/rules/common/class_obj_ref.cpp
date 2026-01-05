//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/class_obj_ref.h"

#include "tm_parse/parser/rules/common/full_object_ref.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"

namespace tm_parse::rules {

ClassObjectRef::ClassObjectRef() = default;
ClassObjectRef::~ClassObjectRef() = default;
ClassObjectRef::ClassObjectRef(ClassObjectRef&&) noexcept = default;
ClassObjectRef& ClassObjectRef::operator=(ClassObjectRef&&) noexcept = default;

bool ClassObjectRef::matches(Matcher& matcher) noexcept {
    return matcher.matches<PropertyDotIdentifier>()
           && matcher.maybe_real(tk::SingleQuote)
           && FullObjectRef::matches(matcher)
           && matcher.maybe_real(tk::SingleQuote);
}

std::unique_ptr<ClassObjectRef> ClassObjectRef::create(Parser& parser) {
    auto rule = std::make_unique<ClassObjectRef>();

    rule->m_Class = PropertyDotIdentifier::create(parser);
    parser.require_real(tk::SingleQuote);
    rule->m_Object = FullObjectRef::create(parser);
    const Token& last = parser.require_real(tk::SingleQuote);

    rule->post_init(rule->m_Class->first_token(), last);

    rule->m_Class->set_parent(*rule);
    rule->m_Object->set_parent(*rule);
    return rule;
}

const PropertyDotIdentifier& ClassObjectRef::clazz() const {
    return *m_Class;
}

const FullObjectRef& ClassObjectRef::object() const {
    return *m_Object;
}

void ClassObjectRef::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Class->visit(func);
    m_Object->visit(func);
}

void ClassObjectRef::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Object->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules