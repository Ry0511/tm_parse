//
// Date       : 13/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include <any>
#include <utility>

namespace tm_parse {
class Lexer;
}

namespace tm_parse::tests {

class TestRunner;

class TestFile {
   private:
    fs::path m_TestFile;
    str m_TestContent;
    std::unordered_map<str, std::unique_ptr<std::any>> m_TestData;

   public:
    TestFile(const fs::path& test_file);
    ~TestFile() = default;

   private:
    const std::any& get_impl(const str& key) const;

   public:
    template <class T>
    T get(const str& key, auto&& def_val = T{}) const {
        try {
            const std::any& val = get_impl(key);

            if constexpr (std::is_same_v<T, float> || std::is_same_v<T, uint32_t>
                          || std::is_same_v<T, uint64_t> || std::is_same_v<T, int32_t>
                          || std::is_same_v<T, int64_t>) {
                return static_cast<T>(std::any_cast<double>(val));
            }

            return std::any_cast<T>(val);

        } catch (const std::runtime_error&) {
            return def_val;
        }
    }

   public:
    std::unique_ptr<TestRunner> create_test_runner() const;

   public:
    const fs::path& test_file() const noexcept { return m_TestFile; }
    const str& test_type() const noexcept { return get<const str&>("test_type", str{"Unknown"}); }

   public:
    auto begin() const noexcept { return m_TestData.begin(); }
    auto end() const noexcept { return m_TestData.end(); }

   private:
    void process_lexer_values();
};

}  // namespace tm_parse::tests