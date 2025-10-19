//
// Date       : 19/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/dot_identifier.h"
#include "tm_parse/parser/rules/common/full_object_ref.h"

namespace tm_parse::rules {

FullObjectRef::FullObjectRef() noexcept = default;
FullObjectRef::~FullObjectRef() noexcept = default;
FullObjectRef::FullObjectRef(FullObjectRef&&) noexcept = default;
FullObjectRef& FullObjectRef::operator=(FullObjectRef&&) noexcept = default;

bool FullObjectRef::matches(Matcher& matcher) noexcept {
    if (!DotIdentifier::matches(matcher)) {
        return false;
    }

    if (matcher.maybe_real(tk::Colon)) {
        return DotIdentifier::matches(matcher);
    }

    return true;
}

std::unique_ptr<FullObjectRef> FullObjectRef::create(Parser& parser) {
    auto ref = std::make_unique<FullObjectRef>();

    ref->m_MainObject = DotIdentifier::create(parser);
    ref->m_MainObject->set_parent(*ref);

    if (parser.maybe_real(tk::Colon)) {
        ref->m_SubObject = DotIdentifier::create(parser);
        ref->m_SubObject->set_parent(*ref);
        ref->post_init(ref->m_MainObject->first_token(), ref->m_SubObject->last_token());
    }
    // no sub-object this ref is just a dot identifier
    else {
        ref->copy_state(*ref->m_MainObject);
    }

    return ref;
}

void FullObjectRef::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_MainObject->visit(func);

    if (m_SubObject) {
        m_SubObject->visit(func);
    }
}

void FullObjectRef::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_MainObject->cascade_assign_parents(this);

    if (m_SubObject) {
        m_SubObject->cascade_assign_parents(this);
    }
}

}  // namespace tm_parse::rules
