//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class LogInfoExpr : public Expr {
   private:
    str m_Text;

   public:
    LogInfoExpr() noexcept;
    ~LogInfoExpr() noexcept override;

   public:
    LogInfoExpr(const LogInfoExpr&);
    LogInfoExpr& operator=(const LogInfoExpr&);
    LogInfoExpr(LogInfoExpr&&) noexcept;
    LogInfoExpr& operator=(LogInfoExpr&&) noexcept;

   public:
    RULE_STATIC_API(LogInfoExpr);
};

}  // namespace tm_parse::rules