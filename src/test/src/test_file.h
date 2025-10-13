//
// Date       : 13/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "tm_parse/lexer/token.h"

namespace tm_parse {
class Lexer;
class Parser;
}  // namespace tm_parse

namespace tm_parse::tests {

class TestRunner;

class TestFile {
   private:
    fs::path m_TestFile;
    str m_TestContent;
    std::map<str, std::any> m_TestData;

   public:
    TestFile(const fs::path& test_file);
    ~TestFile() = default;

   public:
    const fs::path& test_file() const noexcept { return m_TestFile; }
    const str& test_type() const noexcept { return get<const str&>("test_type", str{"Unknown"}); }
    auto begin() const noexcept { return m_TestData.begin(); }
    auto end() const noexcept { return m_TestData.end(); }

    std::unique_ptr<TestRunner> create_test_runner() const;

   public:
    template <class T>
    T get(const str& key, T dflt = T{}) const {
        try {
            const std::any& val = get_impl(key);

            // clang-format off
            if constexpr (
                std::is_same_v<T, float>
                || std::is_same_v<T, uint32_t>
                || std::is_same_v<T, uint64_t>
                || std::is_same_v<T, int32_t>
                || std::is_same_v<T, int64_t>
            ) {
                return static_cast<T>(std::any_cast<double>(val));
            }
            // clang-format on

            return std::any_cast<T>(val);

        } catch (const std::runtime_error&) {
            return dflt;
        }
    }

   private:
    void read_values(Parser& parser);
    static std::any read_simple(Parser& parser);
    std::any read_block(str_view id, Parser& parser);

   private:
    std::any read_test_content(Parser& parser);
    std::any read_expected_tokens(Parser& parser);
    std::any read_expected_text(Parser& parser);
    std::any read_expected_parse_content(Parser& parser);

    std::any read_generic_block(
        std::span<const tk::TokenKind> skip_tokens,
        Parser& parser,
        str_view* out_text = nullptr
    );
    const std::any& get_impl(const str& key) const;
};

}  // namespace tm_parse::tests