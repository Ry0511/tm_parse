//
// Date       : 07/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/util/text_helpers.h"

namespace tm_parse::txt {

bool is_whitespace(char c) noexcept {
    return std::isspace(static_cast<unsigned char>(c));
}

bool is_alpha(char c) noexcept {
    return std::isalpha(static_cast<unsigned char>(c));
}

bool is_digit(char c) noexcept {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool is_alnum(char c) noexcept {
    return std::isalnum(static_cast<unsigned char>(c));
}

bool is_identifier_start(char c) noexcept {
    return is_alpha(c) || c == TXT('_');
}

}  // namespace tm_parse::txt