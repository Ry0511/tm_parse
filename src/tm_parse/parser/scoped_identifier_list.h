//
// Date       : 10/01/2026
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse {

struct ScopedIdentifierEntry {
    static constexpr ParserRule* global_scope{nullptr};
    static const ScopedIdentifierEntry invalid_entry;
    str Identifier;
    ParserRule* Scope{nullptr};
    ParserRule* Node{nullptr};

    bool operator==(const ScopedIdentifierEntry& rhs) const noexcept {
        return Identifier == rhs.Identifier
               && Scope == rhs.Scope
               && Node == rhs.Node;
    }
};

class ScopedIdentifierList {

   private:
    std::vector<ScopedIdentifierEntry> m_IdentifierList;

   public:
    ScopedIdentifierList() = default;
    ~ScopedIdentifierList() = default;

   public:
    auto size() const noexcept { return m_IdentifierList.size(); }
    auto begin() const noexcept { return m_IdentifierList.begin(); }
    auto end() const noexcept { return m_IdentifierList.end(); }

    ScopedIdentifierEntry find(
        str_view identifier,
        ParserRule* scope = nullptr
    ) const noexcept;

    bool insert(
        str_view identifier,
        ParserRule* scope,
        ParserRule* node
    ) noexcept;
};

}  // namespace tm_parse
