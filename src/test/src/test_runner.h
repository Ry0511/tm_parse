//
// Date       : 14/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"

namespace tm_parse::tests {

class TestFile;

class TestRunner {
   public:
    using TokenVec = std::vector<Token>;

   protected:
    bool m_Success{true};
    std::vector<str> m_ExecutionLog;

   public:
    TestRunner() = default;
    virtual ~TestRunner() = default;

   public:
    const std::vector<str>& execution_log() const noexcept { return m_ExecutionLog; }
    bool success() const noexcept { return m_Success; }

   public:
    virtual bool run(TestFile&) = 0;

   protected:
    template <class... Args>
    void log(str_view level, std::format_string<Args...> fmt, Args&&... args) {
        m_ExecutionLog.emplace_back(
            std::format("{} {}", level, std::format(fmt, std::forward<Args>(args)...))
        );
    }

    template <class... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) {
        this->log("\033[32m[I]\033[0m", fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void err(std::format_string<Args...> fmt, Args&&... args) {
        this->log("\033[31m[E]\033[0m", fmt, std::forward<Args>(args)...);
    }
};

}  // namespace tm_parse::tests