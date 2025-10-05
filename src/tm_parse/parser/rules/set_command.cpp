//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "common/dot_identifier.h"
#include "tm_parse/parser/rules/set_command.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/object_ref.h"
#include "tm_parse/parser/rules/common/property_access.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

bool SetCommand::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::Set) && ObjectRef::matches(matcher)
           && PropertyAccess::matches(matcher);
}

std::unique_ptr<SetCommand> SetCommand::create(Parser& parser) {
    auto rule = std::make_unique<SetCommand>();

    Token first = parser.require_next_real(tk::Set);
    rule->m_ObjectRef = ObjectRef::create(parser);
    rule->m_Property = PropertyAccess::create(parser);
    rule->m_Expr = Expr::create(parser);

    rule->m_ObjectRef->set_parent(*rule);
    rule->m_Property->set_parent(*rule);
    rule->m_Expr->set_parent(*rule);

    rule->post_init(first, rule->m_Expr->last_token());

    return rule;
}

void SetCommand::visit(const std::function<void(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
    m_ObjectRef->visit(func);
    m_Property->visit(func);
    m_Expr->visit(func);
}

}  // namespace tm_parse::rules