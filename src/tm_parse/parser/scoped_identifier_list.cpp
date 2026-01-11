//
// Date       : 10/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/scoped_identifier_list.h"

namespace tm_parse {

const ScopedIdentifierEntry ScopedIdentifierEntry::invalid_entry{};

ScopedIdentifierEntry ScopedIdentifierList::find(
    str_view identifier,
    ParserRule* scope
) const noexcept {
    for (const auto& entry : m_IdentifierList) {
        if (
            entry.Scope == scope
            && entry.Identifier == identifier
        ) {
            return entry;
        }
    }
    return ScopedIdentifierEntry::invalid_entry;
}

bool ScopedIdentifierList::insert(
    str_view identifier,
    ParserRule* scope,
    ParserRule* node
) noexcept {
    if (find(identifier, scope) == ScopedIdentifierEntry::invalid_entry) {
        return false;
    }
    m_IdentifierList.emplace_back(str{identifier}, scope, node);
    return true;
}

}  // namespace tm_parse
