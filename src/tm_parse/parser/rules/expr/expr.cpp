//
// Date       : 04/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/expr.h"
#include "tm_parse/parser/rules/util/common_expr.h"

namespace tm_parse::rules {

bool Expr::matches(Matcher& matcher) noexcept {
    return all_expressions{}.matches(matcher);
}

std::unique_ptr<ParserRule> Expr::create(Parser& parser) {
    return all_expressions{}.create(parser);
}

}  // namespace tm_parse::rules
