//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include <format>
#include <source_location>

namespace tm_parse {
struct SrcLoc;
}

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
        ::tm_parse::SrcLoc::current()         \
    )
// clang-format on

#define LOG_TRACE(...) TM_PARSE_LOG(Trace, __VA_ARGS__)
#define LOG_INFO(...) TM_PARSE_LOG(Info, __VA_ARGS__)
#define LOG_WARN(...) TM_PARSE_LOG(Warn, __VA_ARGS__)
#define LOG_ERR(...) TM_PARSE_LOG(Err, __VA_ARGS__)

using LogCallback = std::function<void(LogLevel, std::string_view, const SrcLoc&)>;

std::string_view get_log_level_name(LogLevel level, bool no_colour = false);

void log(LogLevel level, std::string_view msg, const SrcLoc& src);
void add_log_callback(const LogCallback& callback);

}  // namespace tm_parse::logging
