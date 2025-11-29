//
// Date       : 27/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"

namespace tm_parse {

class TokenError : public std::runtime_error {
   private:
    Token m_Token;
    SrcLoc m_SourceLocation;

   public:
    TokenError(const Token& token, const SrcLoc& = SrcLoc::current());
    TokenError(const std::string& msg, const Token& token, const SrcLoc& = SrcLoc::current());
    ~TokenError() override = default;

   public:
    const Token& token() const noexcept { return m_Token; }
    void log_error() const noexcept;
};

}  // namespace tm_parse