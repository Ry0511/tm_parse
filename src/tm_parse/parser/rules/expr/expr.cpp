//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/Expr.h"

namespace tm_parse::rules {

bool Expr::matches(Matcher& matcher) noexcept {
    return true;
}

std::unique_ptr<Expr> Expr::create(Parser& parser) {
    return nullptr;
}

}  // namespace tm_parse::rules
