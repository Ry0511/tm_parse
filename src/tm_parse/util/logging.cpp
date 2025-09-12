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
        case LogLevel::Info:  return "\033[32mINFO \033[0m"; // Green
        case LogLevel::Warn:  return "\033[33mWARN \033[0m"; // Yellow
        case LogLevel::Err:   return "\033[31mERROR\033[0m"; // Red
        default:              return "\033[34mIDK  \033[0m"; // Blue
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

std::string_view get_function_name(const std::source_location& src) {
    // Just grab the actual function name since it logs the full function
    // i.e.,
    //   _cdecl void my_namespace::my_function(const int&)
    const char* func = src.function_name();
    const char* start = func;
    const char* end = func;

    // clang-format off
    while (*end != '(')                    { ++end;   } start = end;
    while (*start != ':' && *start != ' ') { --start; } ++start;
    // clang-format on

    return std::string_view{start, end};
}

}  // namespace

void log(LogLevel level, std::string_view msg, const std::source_location& src) {
    std::string_view file_name = get_file_name(src);
    std::string_view function_name = get_function_name(src);

    std::string header = std::format(
        "[{:>5}] {:>60}:{:<3}",
        get_log_level_name(level),
        truncate_left(std::format("\033[34m{}\033[0m {}", function_name, file_name), 60),
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
