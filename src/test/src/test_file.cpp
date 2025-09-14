//
// Date       : 13/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "test_file.h"

#include <algorithm>

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::tests {

TestFile::TestFile(const fs::path& test_file) : m_TestFile(test_file) {
    if (!fs::is_regular_file(test_file)) {
        throw std::runtime_error(std::format("file not found {}", test_file.string()).c_str());
    }

    using It = std::istreambuf_iterator<str_char>;
    str_ifstream ss{test_file};
    m_TestContent = str{It{ss}, It{}};

    Lexer lexer{m_TestContent};

    while (!lexer.is_eof()) {
        Token id = lexer.next_real_token();

        if (id != tk::Identifier) {
            throw std::runtime_error{
                std::format("unexpected token in test file: {}", id.to_string()).c_str()
            };
        }

        lexer.require_next_real(tk::Equal);
        Token val = lexer.next_real_token();
        str key = str{id.text()};

        if (val == tk::LeftBrace) {
            m_TestData[key] = std::make_unique<std::any>(lexer);
            while (lexer.next_token() != tk::RightBrace) {}
        }
        // Read bool value
        else if (val == tk::True || val == tk::False) {
            m_TestData[key] = std::make_unique<std::any>(val == tk::True);
        }
        // Read number value
        else if (val == tk::Number) {
            m_TestData[key] = std::make_unique<std::any>(txt::parse_number(val.text()));
        }
        // Consume to BlankLine as a string
        else {
            TextRegion region = val.Region;
            do {
                region = region.extend(val.Region);
            } while ((val = lexer.next_token()) != tk::BlankLine);

            m_TestData[key] =
                std::make_unique<std::any>(str{region.create_str_view(m_TestContent)});
        }
    }

    // Second pass go through any Lexer values and parse the internal data
    for (auto& [key, val] : m_TestData) {
        if (val->type() != typeid(Lexer)) {
            continue;
        }

        Lexer lexer = std::any_cast<Lexer>(*val);

        std::vector<Token> tokens{};

        std::vector<tk::TokenKind> skip_tokens{};

        // Skip everything in the expected output block
        if (key == "expected_output") {
            skip_tokens = {
                tk::BlankLine,
                tk::LineComment,
                tk::MultiLineComment,
            };
        }
        // optional skip tokens
        else {
            if (this->get<bool>("skip_comments", true)) {
                skip_tokens.push_back(tk::LineComment);
                skip_tokens.push_back(tk::MultiLineComment);
            }

            if (this->get<bool>("skip_blank_lines", true)) {
                skip_tokens.push_back(tk::BlankLine);
            }
        }

        Token tk = lexer.next_token();

        while (tk != tk::RightBrace) {
            bool skip =
                std::ranges::any_of(skip_tokens, [&](tk::TokenKind token) { return tk == token; });

            if (!skip) {
                tokens.push_back(tk);
            }

            tk = lexer.next_token();
        }

        *m_TestData[key] = std::move(tokens);
    }
}

const std::any& TestFile::get_impl(const str& key) const {
    auto it = m_TestData.find(key);

    if (it == m_TestData.end()) {
        throw std::runtime_error{std::format("key {} not found", key).c_str()};
    }

    return *it->second;
}

}  // namespace tm_parse::tests
