//
// Date       : 07/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::txt {

str_char to_lower(str_char c) noexcept {
    if constexpr (std::is_same_v<str_char, char>) {
        return static_cast<str_char>(std::tolower(c));
    } else {
        return static_cast<str_char>(std::towlower(c));
    }
}

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
    if (a.size() != b.size()) {
        return false;
    }
    return std::equal(a.begin(), a.end(), b.begin(), [](const auto& a, const auto& b) {
        return to_lower(a) == to_lower(b);
    });
}

str escape_string(str_view in, bool flatten_whitespace) noexcept {
    str out{};
    out.reserve(in.size() * 2);

    bool last_was_ws = false;

    // clang-format off
    for (char c : in) {

        // bit confusing but if flatten whitespace is enabled we only want the last occurence of
        // whitespace to be appended to the returned string
        if (flatten_whitespace && c == TXT(' ')) {
            last_was_ws = true;
            continue;
        }

        if (flatten_whitespace && last_was_ws && c != TXT(' ')) {
            out.push_back(TXT(' '));
            last_was_ws = false;
        }

        switch (c) {
            case '\t': { out += "\\t"; break; }
            case '\r': { out += "\\r"; break; }
            case '\n': { out += "\\n"; break; }
            case '\0': { out += "\\0"; break; }
            default  : { out += c;     break; }
        }
    }
    // clang-format on

    out.shrink_to_fit();

    return out;
}

str sanitise_string(str_view text) {
    str out{};
    out.reserve(text.size() * 2);
    bool escape_sequence = false;

    for (str_char c : text) {
        // enable escape sequence start
        if (!escape_sequence && c == TXT('\\')) {
            escape_sequence = true;
        }
        // not escaping anything just put the literal char
        else if (!escape_sequence) {
            out.push_back(c);
        }
        // escape sequence; next character is special
        else {
            // Not exhaustive but should cover the common cases
            switch (c) {
                    // clang-format off
                case TXT('\\'): out.push_back(TXT('\\')); break;
                case TXT('n'):  out.push_back(TXT('\n')); break;
                case TXT('r'):  out.push_back(TXT('\r')); break;
                case TXT('t'):  out.push_back(TXT('\t')); break;
                case TXT('"'):
                case TXT('\''): out.push_back(c); break;
                    // clang-format on
                default:
                    throw std::logic_error{
                        std::format("invalid escape sequence \\{} in '{}'", c, text)
                    };
            }
            escape_sequence = false;
        }
    }

    if (escape_sequence) {
        throw std::logic_error{std::format("string has trailing escape sequence; {}", text)};
    }

    out.shrink_to_fit();
    return out;
}

std::optional<double> parse_double(str_view text) noexcept {
    try {
        return std::stod(str{text});
    } catch (const std::logic_error&) {
        return std::nullopt;
    }
}

std::optional<int32_t> parse_int32(str_view text) noexcept {
    try {
        return static_cast<int32_t>(std::stoi(str{text}));
    } catch (const std::logic_error&) {
        return std::nullopt;
    }
}

std::optional<int64_t> parse_int64(str_view text) noexcept {
    try {
        return static_cast<int64_t>(std::stoll(str{text}));
    } catch (const std::logic_error&) {
        return std::nullopt;
    }
}

std::optional<size_t> parse_size_t(str_view text) noexcept {
    using Limits = std::numeric_limits<size_t>;

    // Assumes no leading whitespace
    if (text.empty() || text.front() == TXT('-')) {
        return std::nullopt;
    }

    // decimal value in index
    if (text.find_first_of(TXT('.')) != str::npos) {
        return std::nullopt;
    }

    try {
        auto val = std::stoull(str{text});
        if (std::cmp_less(val, Limits::min()) || std::cmp_greater(val, Limits::max())) {
            return std::nullopt;
        }
        return static_cast<size_t>(val);
    } catch (const std::logic_error&) {
        return std::nullopt;
    }
}

}  // namespace tm_parse::txt
