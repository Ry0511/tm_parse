//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "dot_identifier.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/object_ref.h"

namespace tm_parse::rules {

bool ObjectRef::matches(Matcher& matcher) noexcept {
    if (!DotIdentifier::matches(matcher)) {
        return false;
    }

    if (matcher.maybe(tk::Colon)) {
        return DotIdentifier::matches(matcher);
    }

    return true;
}

std::unique_ptr<ObjectRef> ObjectRef::create(Parser& parser) {
    auto ref = std::make_unique<ObjectRef>();

    ref->m_MainObject = DotIdentifier::create(parser);
    ref->m_MainObject->set_parent(*ref);

    if (parser.maybe(tk::Colon)) {
        ref->m_SubObject = DotIdentifier::create(parser);
        ref->m_SubObject->set_parent(*ref);
        ref->post_init(ref->m_MainObject->first_token(), ref->m_SubObject->last_token());
    } else {
        ref->copy_state(*ref->m_MainObject);
    }

    return ref;
}

ObjectRef::~ObjectRef() = default;
ObjectRef::ObjectRef(ObjectRef&&) noexcept = default;
ObjectRef& ObjectRef::operator=(ObjectRef&&) noexcept = default;

void ObjectRef::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    func(*this);
    this->m_MainObject->visit(func);

    if (this->m_SubObject != nullptr) {
        this->m_SubObject->visit(func);
    }
}

}  // namespace tm_parse::rules