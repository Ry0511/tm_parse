//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include <format>
#include <source_location>

namespace tm_parse::logging {

enum class LogLevel : uint8_t {
    Trace,
    Info,
    Warn,
    Err,
};

// clang-format off
#define TM_PARSE_LOG(level, ...)              \
    ::tm_parse::logging::log(                 \
        ::tm_parse::logging::LogLevel::level, \
        std::format(__VA_ARGS__),             \
        std::source_location::current()       \
    )
// clang-format on

#define LOG_TRACE(...) TM_PARSE_LOG(Trace, __VA_ARGS__)
#define LOG_INFO(...) TM_PARSE_LOG(Info, __VA_ARGS__)
#define LOG_WARN(...) TM_PARSE_LOG(Warn, __VA_ARGS__)
#define LOG_ERR(...) TM_PARSE_LOG(Err, __VA_ARGS__)

void log(LogLevel level, std::string_view msg, const std::source_location& src);

}  // namespace tm_parse::logging
