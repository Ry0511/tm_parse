//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/util/logging.h"
#include "tm_parse/util/src_loc.h"

namespace tm_parse::logging {

namespace {

std::vector<LogCallback> callbacks;
bool running_callbacks{false};

void notify_callbacks(LogLevel level, std::string_view msg, const SrcLoc& src) {
    running_callbacks = true;
    for (const auto& callback : callbacks) {
        callback(level, msg, src);
    }
    running_callbacks = false;
}

std::string_view truncate_left(std::string_view str, size_t len) {
    if (str.size() <= len) {
        return str;
    }
    return str.substr(str.size() - len);
}

}  // namespace

std::string_view get_log_level_name(LogLevel level, bool no_colour) {
    if (!no_colour) {
        // clang-format off
        switch (level) {
            case LogLevel::Trace: return "\033[32mTRACE\033[0m"; // Green
            case LogLevel::Info:  return "\033[32m INFO\033[0m"; // Green
            case LogLevel::Warn:  return "\033[33m WARN\033[0m"; // Yellow
            case LogLevel::Err:   return "\033[31mERROR\033[0m"; // Red
            default:              return "\033[34m  IDK\033[0m"; // Blue
        }
        // clang-format on
    } else {
        // clang-format off
        switch (level) {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Info:  return "INFO";
            case LogLevel::Warn:  return "WARN";
            case LogLevel::Err:   return "ERROR";
            default:              return "IDK";
        }
        // clang-format on
    }
}

void log(LogLevel level, std::string_view msg, const SrcLoc& src) {
    std::string_view file_name = src.file_name();
    std::string function_name = src.function();

    std::string header = std::format(
        "[{:>5}] {:>60}:{:0>4} ",
        get_log_level_name(level),
        truncate_left(std::format("{} {}", function_name, file_name), 60),
        src.line()
    );

    if (!running_callbacks) {
        notify_callbacks(level, msg, src);
    }

    size_t begin{0};
    for (size_t i = 0; i <= msg.size(); ++i) {
        if (i < msg.size()) {
            if (msg[i] != '\n' && msg[i] != '\r') {
                continue;
            }
        }

        str_view line = msg.substr(begin, i - begin);
        begin = i + 1;

        if (line.empty()) {
            continue;
        }

        std::cout << header << line << '\n';
    }
}

void add_log_callback(const LogCallback& callback) {
    callbacks.push_back(callback);
}

}  // namespace tm_parse::logging
