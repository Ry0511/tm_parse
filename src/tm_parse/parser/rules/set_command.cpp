//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/rules/set_command.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/dot_identifier.h"
#include "tm_parse/parser/rules/object_ref.h"

namespace tm_parse::rules {

void SetCommand::visit(const std::function<bool(const ParserRule&)>& func) const noexcept {
    ParserRule::visit(func);
}

bool SetCommand::matches(Parser& parser) noexcept {
    return true;
}

std::unique_ptr<SetCommand> SetCommand::create(Parser& parser) {
    SetCommand rule{};

    Token first = parser.require_next_real(tk::Set);
    rule.m_ObjectRef = ObjectRef::create(parser);
    rule.m_Property = DotIdentifier::create(parser);
    rule.post_init(first, rule.m_Property->last_token());

    return std::make_unique<SetCommand>(std::move(rule));
}

}  // namespace tm_parse::rules