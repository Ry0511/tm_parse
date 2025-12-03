//
// Date       : 16/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse {

// TODO: This isn't fixed to just Expressions its any list of rules

template <class... Rules>
    requires(sizeof...(Rules) > 0 && (std::derived_from<Rules, ParserRule> && ...))
struct ExprTypeList {
    constexpr ExprTypeList() noexcept = default;
    constexpr ~ExprTypeList() noexcept = default;
    std::unique_ptr<ParserRule> create(Parser& p) const { return p.create_one_of<Rules...>(); }
    bool matches(Matcher& m) const noexcept { return m.matches_one_of<Rules...>(); }
};

}  // namespace tm_parse
