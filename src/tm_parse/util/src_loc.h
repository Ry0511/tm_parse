//
// Date       : 29/11/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include <source_location>
#include <string>

namespace tm_parse {

struct SrcLoc {
    using Impl = std::source_location;
    Impl Src;

    constexpr explicit SrcLoc(const Impl& src = Impl::current())
        : Src(src) {}
    constexpr ~SrcLoc() noexcept = default;

    static consteval SrcLoc current(const Impl& src = Impl::current()) noexcept {
        return SrcLoc{src};
    }

    constexpr auto line() const noexcept { return Src.line(); }
    constexpr auto column() const noexcept { return Src.column(); }

    constexpr std::string_view file_name() const noexcept {
        const char* abs_path = Src.file_name();
        const char* start = abs_path;
        const char* end = abs_path;

        // we only care about the filename
        while (*end != '\0') {
            char c = *end;
            if (c == '\\' || c == '/') {
                start = end + 1;
            }
            ++end;
        }
        return std::string_view{start, end};
    }

    constexpr std::string function() const noexcept {
        const char* func = Src.function_name();
        const char* start = func;
        const char* end = func;

        // clang-format off
        while (*end != '(')   { ++end;   } start = end;
        while (*start != ' ') { --start; } ++start;
        // clang-format on

        std::string_view func_view{start, end};
        std::string func_str{};
        func_str.reserve(func_view.size());

        for (size_t i = 0; i < func_view.size(); ++i) {
            if (func_view.at(i) == '`') {
                func_str += "$:";
                i += sizeof("`anonymous-namespace`") - 1;
            } else {
                func_str += func_view.at(i);
            }
        }

        return func_str;
    }
};

}  // namespace tm_parse
