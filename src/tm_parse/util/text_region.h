//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse {

struct TextRegion {
    size_t Start;
    size_t End;

    constexpr TextRegion() noexcept
        : Start(invalid_index_v), End(invalid_index_v) {}
    constexpr TextRegion(size_t start, size_t end) noexcept
        : Start(start), End(end) {}

    size_t length() const noexcept { return End - Start; }
    bool is_empty() const noexcept { return Start == End; }

    str_view create_str_view(const str_char* txt) const noexcept {
        return str_view{txt + Start, length()};
    }

    str_view create_str_view(str_view vw) const noexcept { return vw.substr(Start, length()); }
    str create_str(str_view vw) const noexcept { return str{vw.substr(Start, length())}; }

    TextRegion extend(const TextRegion& o) const noexcept {
        return TextRegion{std::min(Start, o.Start), std::max(End, o.End)};
    }
    bool operator==(const TextRegion& other) const noexcept {
        return Start == other.Start && End == other.End;
    }
    bool operator!=(const TextRegion& other) const noexcept { return !(*this == other); }

    operator bool() const noexcept { return operator==(TextRegion{}); }
};

}  // namespace tm_parse