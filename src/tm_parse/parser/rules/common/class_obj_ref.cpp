//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/class_obj_ref.h"
#include "tm_parse/parser/rules/common/obj_dot_identifier.h"

namespace tm_parse::rules {

ClassObjectRef::ClassObjectRef() = default;
ClassObjectRef::~ClassObjectRef() = default;
ClassObjectRef::ClassObjectRef(ClassObjectRef&&) noexcept = default;
ClassObjectRef& ClassObjectRef::operator=(ClassObjectRef&&) noexcept = default;

bool ClassObjectRef::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::AnyIdentifier)
           && matcher.maybe_real(tk::SingleQuote)
           && ObjectDotIdentifier::matches(matcher)
           && matcher.maybe_real(tk::SingleQuote);
}

std::unique_ptr<ClassObjectRef> ClassObjectRef::create(Parser& parser) {
    Token first = parser.require_real(tk::AnyIdentifier);
    parser.require_real(tk::SingleQuote);

    auto rule = std::make_unique<ClassObjectRef>();
    rule->m_Class = str{first.text()};
    rule->m_Object = ObjectDotIdentifier::create(parser);
    Token last = parser.require_real(tk::SingleQuote);

    rule->post_init(first, last);
    return rule;
}

void ClassObjectRef::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Object->visit(func);
}

void ClassObjectRef::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Object->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules