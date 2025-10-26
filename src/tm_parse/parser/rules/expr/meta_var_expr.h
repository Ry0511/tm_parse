//
// Date       : 08/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/parser/rules/expr/expr.h"

namespace tm_parse::rules {

class PropertyDotIdentifier;

class MetaVarExpr : public Expr {
   private:
    std::unique_ptr<PropertyDotIdentifier> m_Identifier;

   public:
    MetaVarExpr() noexcept;
    ~MetaVarExpr() noexcept override;

   public:
    MetaVarExpr(const MetaVarExpr&) = delete;
    MetaVarExpr& operator=(const MetaVarExpr&) = delete;
    MetaVarExpr(MetaVarExpr&&) noexcept;
    MetaVarExpr& operator=(MetaVarExpr&&) noexcept;

   public:
    RULE_STATIC_API(MetaVarExpr);

   public:
    void visit(const std::function<void(const ParserRule&)>& func) const noexcept override;
    void cascade_assign_parents(ParserRule* parent) noexcept override;
};

}  // namespace tm_parse::rules