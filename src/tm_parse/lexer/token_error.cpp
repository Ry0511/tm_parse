//
// Date       : 27/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token_error.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse {

TokenError::TokenError(const Token& token) : std::runtime_error(""), m_Token(token) {}

TokenError::TokenError(const std::string& msg, const Token& token)
    : std::runtime_error(msg),
      m_Token(token) {}

void TokenError::log_error() const noexcept {

    const Token& tk = token();

    LOG_ERR("Token Error ~ {}", this->what());
    LOG_ERR(" at  {}:{}", tk.Line, tk.Column);
    LOG_ERR(" got {}", tk.token_name());
    LOG_ERR(" in  {}", txt::escape_string(tk.text()));

}

}  // namespace tm_parse