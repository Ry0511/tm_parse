//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include <format>

namespace tm_parse {
struct SrcLoc;
}

namespace tm_parse::logging {

enum class LogLevel : uint8_t {
    Trace = 0,
    Info,
    Warn,
    Err,
};

constexpr bool colour_coded_logging = true;

constexpr std::string_view log_level_names[]{
    "TRACE",
    "INFO",
    "WARN",
    "ERR",
};

constexpr std::string_view log_level_names_with_colours[]{
    "\033[32mTRACE\033[0m",
    "\033[32mINFO\033[0m",
    "\033[33mWARN\033[0m",
    "\033[31mERROR\033[0m",
};

constexpr std::string_view get_log_level_name(LogLevel level, bool colour_coded = colour_coded_logging) {
    auto i = static_cast<uint8_t>(level);
    // NOLINTNEXTLINE
    return colour_coded ? log_level_names_with_colours[i] : log_level_names[i];
}

#define TM_PARSE_LOG(level, ...)              \
    ::tm_parse::logging::log(                 \
        ::tm_parse::logging::LogLevel::level, \
        std::format(__VA_ARGS__),             \
        ::tm_parse::SrcLoc::current()         \
    )

#define LOG_TRACE(...) TM_PARSE_LOG(Trace, __VA_ARGS__)
#define LOG_INFO(...) TM_PARSE_LOG(Info, __VA_ARGS__)
#define LOG_WARN(...) TM_PARSE_LOG(Warn, __VA_ARGS__)
#define LOG_ERR(...) TM_PARSE_LOG(Err, __VA_ARGS__)

using LogCallback = std::function<void(LogLevel level, std::string_view msg, const SrcLoc&)>;
void log(LogLevel level, std::string_view msg, const SrcLoc& src);
void set_log_callback(LogCallback callback);

}  // namespace tm_parse::logging
