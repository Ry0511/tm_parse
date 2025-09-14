//
// Date       : 07/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::txt {

bool is_whitespace(str_char c) noexcept;

bool is_alpha(str_char c) noexcept;

bool is_digit(str_char c) noexcept;

bool is_alnum(str_char c) noexcept;

bool is_identifier(str_char c) noexcept;

bool is_newline(str_char c) noexcept;

bool equal_icase(str_view a, str_view b) noexcept;

str escape_string(str_view str) noexcept;

double parse_number(str_view str) noexcept;

}  // namespace tm_parse::txt
