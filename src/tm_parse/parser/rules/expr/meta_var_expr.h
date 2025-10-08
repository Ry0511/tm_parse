//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class DotIdentifier;

class MetaVarExpr : public Expr {
   private:
    std::unique_ptr<DotIdentifier> m_Identifier;

   public:
    MetaVarExpr() noexcept;
    ~MetaVarExpr() noexcept override;

   public:
    MetaVarExpr(const MetaVarExpr&) = delete;;
    MetaVarExpr& operator=(const MetaVarExpr&) = delete;;
    MetaVarExpr(MetaVarExpr&&) noexcept;
    MetaVarExpr& operator=(MetaVarExpr&&) noexcept;

   public:
    RULE_STATIC_API(MetaVarExpr);
};

}  // namespace tm_parse::rules