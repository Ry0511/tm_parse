//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/class_obj_ref.h"
#include "tm_parse/parser/rules/common/full_object_ref.h"
#include "tm_parse/parser/rules/common/object_ref.h"
#include "tm_parse/parser/rules/expr/meta_var_expr.h"

namespace tm_parse::rules {

ObjectRef::~ObjectRef() = default;
ObjectRef::ObjectRef(ObjectRef&&) noexcept = default;
ObjectRef& ObjectRef::operator=(ObjectRef&&) noexcept = default;

bool ObjectRef::matches(Matcher& matcher) noexcept {

    if (matcher.matches<MetaVarExpr>()) {
        return true;
    }

    if (matcher.matches<ClassObjectRef>()) {
        return true;
    }

    return matcher.matches<FullObjectRef>();
}

std::unique_ptr<ObjectRef> ObjectRef::create(Parser& parser) {
    auto ref = std::make_unique<ObjectRef>();
    Matcher m = parser.create_matcher();

    // $(Globals) - builtin meta variable
    if (m.matches<MetaVarExpr>()) {
        ref->m_Ref = MetaVarExpr::create(parser);
    }
    // Class'foo.baz.bar'
    else if (m.matches<ClassObjectRef>()) {
        ref->m_Ref = ClassObjectRef::create(parser);
    }
    // foo.baz:bar
    else {
        ref->m_Ref = FullObjectRef::create(parser);
    }

    ref->m_Ref->set_parent(*ref);
    ref->copy_state(*ref->m_Ref);
    return ref;
}

void ObjectRef::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_Ref->visit(func);
}

void ObjectRef::cascade_assign_parents(ParserRule* parent) noexcept {
    ParserRule::cascade_assign_parents(parent);
    m_Ref->cascade_assign_parents(this);
}

}  // namespace tm_parse::rules