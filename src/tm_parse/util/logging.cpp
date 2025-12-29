//
// Date       : 12/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/util/logging.h"
#include "tm_parse/util/src_loc.h"

namespace tm_parse::logging {

namespace {

LogCallback log_callback{};
std::atomic_bool log_callback_active{false};

std::string_view truncate_left(std::string_view str, size_t len) {
    if (str.size() <= len) {
        return str;
    }
    return str.substr(str.size() - len);
}

}  // namespace

void log(LogLevel level, std::string_view msg, const SrcLoc& src) {
    const std::string_view file_name = src.file_name();
    const std::string& function_name = src.function();

    const std::string& header = std::format(
        "[{:>5}] {:>60}:{:0>4} ",
        get_log_level_name(level, colour_coded_logging),
        truncate_left(std::format("{} {}", function_name, file_name), 60),
        src.line()
    );

    size_t begin{0};
    for (size_t i = 0; i <= msg.size(); ++i) {
        if (i < msg.size()) {
            if (msg.at(i) != '\n' && msg.at(i) != '\r') {
                continue;
            }
        }

        str_view line = msg.substr(begin, i - begin);
        begin = i + 1;

        if (line.empty()) {
            continue;
        }

        if (log_callback.operator bool()) {
            if (!log_callback_active.load()) {
                log_callback_active.store(true);
                log_callback(level, line, src);
                log_callback_active.store(false);
            }
        }

        std::cout << header << line << '\n';
    }
}

void set_log_callback(LogCallback callback) {
    log_callback = std::move(callback);
}

}  // namespace tm_parse::logging
