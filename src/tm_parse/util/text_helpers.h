//
// Date       : 07/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse::txt {

bool is_whitespace(char c) noexcept;

bool is_alpha(char c) noexcept;

bool is_digit(char c) noexcept;

bool is_alnum(char c) noexcept;

bool is_identifier_start(char c) noexcept;

}  // namespace tm_parse::txt
