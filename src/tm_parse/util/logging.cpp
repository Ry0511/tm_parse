//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/util/logging.h"

namespace tm_parse::logging {

namespace {

std::string_view get_log_level_name(LogLevel level) {
    // clang-format off
    switch (level) {
        case LogLevel::Trace: return "\033[32mTRACE\033[0m"; // Green
        case LogLevel::Info:  return "\033[32m INFO\033[0m"; // Green
        case LogLevel::Warn:  return "\033[33m WARN\033[0m"; // Yellow
        case LogLevel::Err:   return "\033[31mERROR\033[0m"; // Red
        default:              return "\033[34m  IDK\033[0m"; // Blue
    }
    // clang-format on
}

std::string_view truncate_left(std::string_view str, size_t len) {
    if (str.size() <= len) {
        return str;
    }
    return str.substr(str.size() - len);
}

std::string_view get_file_name(const std::source_location& src) {
    // Takes an absolute path and returns only the filename
    const char* file_name = src.file_name();
    const char* start = file_name;
    const char* end = file_name;

    while (*end != '\0') {
        char c = *end;
        if (c == '\\' || c == '/') {
            start = end + 1;
        }
        ++end;
    }
    return std::string_view{start, end};
}

std::string get_function_name(const std::source_location& src) {
    const char* func = src.function_name();
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
        if (func_view[i] == '`') {
            func_str += "`anon':";
            i += sizeof("`anonymous-namespace`") - 1;
        } else {
            func_str += func_view[i];
        }
    }

    return func_str;
}

}  // namespace

void log(LogLevel level, std::string_view msg, const std::source_location& src) {
    std::string_view file_name = get_file_name(src);
    std::string function_name = get_function_name(src);

    std::string header = std::format(
        "[{:>5}] {:>60}:{:<3}",
        get_log_level_name(level),
        truncate_left(std::format("{} {}", function_name, file_name), 60),
        src.line()
    );

    const char* end = msg.data();
    const char* start = end;

    while (*end != '\0') {
        char c = *end;
        if (c == '\n' || c == '\r') {
            std::cout << header << " | " << std::string_view{start, end} << '\n';
            start = end + 1;
        }
        ++end;
    }
}

}  // namespace tm_parse::logging
