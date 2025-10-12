//
// Date       : 13/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "lexer_test_runner.h"
#include "test_file.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::tests {

TestFile::TestFile(const fs::path& test_file) : m_TestFile(test_file) {
    if (!fs::is_regular_file(test_file)) {
        throw std::runtime_error(std::format("file not found {}", test_file.string()).c_str());
    }

    using It = std::istreambuf_iterator<str_char>;
    str_ifstream ss{test_file};
    m_TestContent = str{It{ss}, It{}};

    Parser parser{m_TestContent};
    read_values(parser);
}

const std::any& TestFile::get_impl(const str& key) const {
    auto it = m_TestData.find(key);
    if (it == m_TestData.end()) {
        throw std::runtime_error{std::format("key {} not found", key).c_str()};
    }
    return it->second;
}

std::unique_ptr<TestRunner> TestFile::create_test_runner() const {
    const str& test_runner = this->test_type();

    if (txt::equal_icase(test_runner, "LexerTest")) {
        return std::make_unique<LexerTestRunner>();
    }

    return nullptr;
}

void TestFile::read_values(Parser& parser) {
    constexpr tk::TokenKind simple_value_tokens[]{
        tk::True,
        tk::False,
        tk::Number,
        tk::StringLiteral,
        tk::AnyIdentifier
    };

    do {
        // isn't currently handled by the is_eof function
        if (parser.maybe_real(tk::EndOfInput)) {
            break;
        }

        Token id = parser.require_real(tk::AnyIdentifier);
        str text_id = str{id.text()};
        parser.require_real(tk::Equal);

        Matcher m = parser.create_matcher();
        if (Token value = m.any_real(simple_value_tokens)) {
            m_TestData[text_id] = read_simple(parser);
        }
        // A = { ... }
        else if (parser.maybe_real(tk::LeftBrace)) {
            m_TestData[text_id] = read_block(text_id, parser);
            parser.require_real(tk::RightBrace);
        } else {
            Token cur = parser.next_real();
            throw TokenError{
                std::format(
                    "unknown token found when parsing test file: '{}'",
                    m_TestFile.string()
                ),
                cur
            };
        }

    } while (!parser.is_eof());
}

std::any TestFile::read_simple(Parser& parser) {
    Token cur = parser.next_real();

    if (cur == tk::StringLiteral) {
        return std::make_any<str>(cur.inner_text());
    }

    if (cur == tk::True || cur == tk::False) {
        return std::make_any<bool>(cur == tk::True);
    }

    // Sequence of identifiers
    if (cur == tk::AnyIdentifier) {
        Token first = cur;
        Token last = cur;
        while (Token cur = parser.maybe(tk::AnyIdentifier)) {
            last = cur;
        }

        return std::make_any<str>(first.extend(last).create_str(first.Text));
    }

    if (cur == tk::Number) {
        const str_char* data = cur.text().data();
        return std::make_any<double>(static_cast<double>(std::strtod(data, nullptr)));
    }

    throw std::runtime_error{std::format("unsupported token {}", cur.token_name())};
}

std::any TestFile::read_block(str_view id, Parser& parser) {
    // clang-format off
    if (id == TXT("test_content")) {
        return read_test_content(parser);
    }
    else if (id == TXT("expected_tokens")) {
        return read_expected_tokens(parser);
    }
    else if (id == TXT("expected_text")) {
        return read_expected_text(parser);
    }
    else if (id == TXT("expected_parse_content")) {
        return read_expected_parse_content(parser);
    }
    // clang-format on

    throw std::runtime_error{std::format("unknown test block item '{}'", id)};
}

namespace {
constexpr tk::TokenKind default_skip_tokens[]{tk::BlankLine, tk::LineComment, tk::MultiLineComment};
}

std::any TestFile::read_test_content(Parser& parser) {
    std::vector<tk::TokenKind> skip_tokens;

    if (get<bool>(TXT("skip_blank_lines"), true)) {
        skip_tokens.emplace_back(tk::BlankLine);
    }

    if (get<bool>(TXT("skip_comments"), true)) {
        skip_tokens.emplace_back(tk::LineComment);
        skip_tokens.emplace_back(tk::MultiLineComment);
    }

    return read_generic_block(skip_tokens, parser);
}

std::any TestFile::read_expected_tokens(Parser& parser) {
    return read_generic_block(default_skip_tokens, parser);
}

std::any TestFile::read_expected_text(Parser& parser) {
    return read_generic_block(default_skip_tokens, parser);
}

std::any TestFile::read_expected_parse_content(Parser& parser) {
    throw std::runtime_error{"not implemented"};
}

std::any TestFile::read_generic_block(std::span<const tk::TokenKind> skip_tokens, Parser& parser) {
    bool should_exit = false;
    std::vector<Token> tokens{};
    tokens.reserve(512);

    bool skip_blank_lines = false;

    for (const auto& token : skip_tokens) {
        if (token == tk::BlankLine) {
            skip_blank_lines = true;
        }
    }

    do {
        Token cur = parser.next();

        bool should_skip = false;
        for (const auto& kind : skip_tokens) {
            if (cur == kind) {
                should_skip = true;
            }
        }

        if (should_skip) {
            continue;
        }

        if (cur.is_eof()) {
            throw std::runtime_error{"unexpected eof when parsing block"};
        }

        Matcher m = parser.create_matcher();
        should_exit = skip_blank_lines ? m.maybe_real(tk::RightBrace) : m.maybe(tk::RightBrace);
        tokens.push_back(cur);
    } while (!should_exit);

    tokens.shrink_to_fit();

    return tokens;
}

}  // namespace tm_parse::tests
