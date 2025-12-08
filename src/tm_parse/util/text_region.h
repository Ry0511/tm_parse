//
// Date       : 29/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

namespace tm_parse {

using text_region_int = uint32_t;

struct TextRegion {
    text_region_int Start;
    text_region_int End;

    constexpr TextRegion() noexcept
        : Start(invalid_index_v), End(invalid_index_v) {}

    constexpr TextRegion(text_region_int start, text_region_int end) noexcept
        : Start(start), End(end) {}

    constexpr text_region_int length() const noexcept { return End - Start; }
    constexpr bool is_empty() const noexcept { return Start == End; }

    constexpr str_view create_str_view(const str_char* txt) const noexcept {
        return str_view{txt + Start, length()};
    }

    constexpr str_view create_str_view(str_view vw) const noexcept { return vw.substr(Start, length()); }
    constexpr str create_str(str_view vw) const noexcept { return str{vw.substr(Start, length())}; }

    constexpr TextRegion extend(const TextRegion& other) const noexcept {
        return TextRegion{
            std::min(Start, other.Start),
            std::max(End, other.End)
        };
    }

    constexpr bool operator==(const TextRegion& other) const noexcept {
        return (Start == other.Start) && (End == other.End);
    }

    constexpr bool operator!=(const TextRegion& other) const noexcept {
        return this->operator==(other);
    }

    constexpr operator bool() const noexcept {
        return this->operator!=(TextRegion{invalid_index_v, invalid_index_v});
    }
};

}  // namespace tm_parse