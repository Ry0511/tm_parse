//
// Date       : 07/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/util/text_helpers.h"
namespace tm_parse::txt {

bool is_whitespace(str_char c) noexcept {
    switch (c) {
        case TXT('\v'):
        case TXT('\t'):
        case TXT('\r'):
        case TXT(' '):
            return true;
        default:
            return false;
    }

    return std::isspace(static_cast<int>(c));
}

bool is_alpha(str_char c) noexcept {
    return std::isalpha(static_cast<int>(c));
}

bool is_digit(str_char c) noexcept {
    return std::isdigit(static_cast<int>(c));
}

bool is_alnum(str_char c) noexcept {
    return std::isalnum(static_cast<int>(c));
}

bool is_identifier(str_char c) noexcept {
    return is_alnum(c) || c == TXT('_');
}

bool is_newline(str_char c) noexcept {
    return c == TXT('\n') || c == TXT('\r');
}

bool equal_icase(str_view a, str_view b) noexcept {
    return a.size() == b.size()
           && std::equal(a.begin(), a.end(), b.begin(), [](auto&& a, auto&& b) {
                  return std::tolower(a) == std::tolower(b);
              });
}

str escape_string(str_view in) noexcept {
    str out{};
    out.reserve(in.size());

    for (char c : in) {
        switch (c) {
            case '\n': {
                out += "\\n";
                break;
            }
            case '\r': {
                out += "\\r";
                break;
            }
            default: {
                out += c;
                break;
            }
        }
    }

    return out;
}

double parse_number(str_view str) noexcept {
    return std::stod(std::string{str});
}

}  // namespace tm_parse::txt
