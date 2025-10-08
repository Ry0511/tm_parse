//
// Date       : 06/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class DotIdentifier;

class IdentifierRefExpr : public Expr {
   private:
    std::unique_ptr<DotIdentifier> m_Identifier;

   public:
    IdentifierRefExpr();
    ~IdentifierRefExpr() override;

   public:
    IdentifierRefExpr(const IdentifierRefExpr&) = delete;
    IdentifierRefExpr& operator=(const IdentifierRefExpr&) = delete;
    IdentifierRefExpr(IdentifierRefExpr&&) noexcept;
    IdentifierRefExpr& operator=(IdentifierRefExpr&&) noexcept;

   public:
    const DotIdentifier& identifier() const noexcept { return *m_Identifier; }

   public:
    RULE_STATIC_API(IdentifierRefExpr);
};

}  // namespace tm_parse::rules