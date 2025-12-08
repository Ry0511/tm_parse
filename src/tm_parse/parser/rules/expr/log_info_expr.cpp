//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/expr/expr.h"
#include "tm_parse/util/text_helpers.h"

#include "tm_parse/parser/rules/expr/log_info_expr.h"

namespace tm_parse::rules {

LogInfoExpr::LogInfoExpr() noexcept = default;
LogInfoExpr::~LogInfoExpr() noexcept = default;

LogInfoExpr::LogInfoExpr(const LogInfoExpr&) = default;
LogInfoExpr& LogInfoExpr::operator=(const LogInfoExpr&) = default;
LogInfoExpr::LogInfoExpr(LogInfoExpr&&) noexcept = default;
LogInfoExpr& LogInfoExpr::operator=(LogInfoExpr&&) noexcept = default;

bool LogInfoExpr::matches(Matcher& matcher) noexcept {
    return matcher.maybe_real(tk::LogInfo) && matcher.maybe_real(tk::StringLiteral);
}

std::unique_ptr<LogInfoExpr> LogInfoExpr::create(Parser& parser) {
    // TODO: Strings are not unescaped currently so will want to convert \n and what not into their
    //  actual character
    Token first = parser.require_real(tk::LogInfo);
    Token last = parser.require_real(tk::StringLiteral);

    auto rule = std::make_unique<LogInfoExpr>();
    rule->m_Text = str{last.inner_text()};
    rule->post_init(first, last);

    return rule;
}

}  // namespace tm_parse::rules
