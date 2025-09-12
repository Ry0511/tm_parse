//
// Date       : 11/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::tests {

namespace {

struct TestInfo {
    str TestText;
    str TestType;
    std::vector<Token> TestContent;
    std::vector<Token> ExpectedOutput;
};

str read_file(const fs::path& file) {
    using It = std::istreambuf_iterator<str_char>;
    str_ifstream ss{file};
    return str{It{ss}, It{}};
}

TestInfo parse_test_info(const fs::path& test_file) {
    TestInfo test_info{};
    test_info.TestText = read_file(test_file);
    Lexer lexer{test_info.TestText};

    while (!lexer.is_eof()) {
        Token tk = lexer.next_token();

        // Grab test type
        if (tk == tk::Identifier && txt::equal_icase(tk.text(), "test_type")) {
            lexer.require(tk::Equal);
            Token test_type = lexer.require(tk::Identifier);
            test_info.TestType = str{test_type.text()};
        }
        // Grab test content
        else if (tk == tk::Identifier && txt::equal_icase(tk.text(), "test_content")) {
            lexer.require(tk::Equal);
            lexer.require(tk::LeftBrace);

            bool terminator_found = false;
            while (!terminator_found) {
                Token content_token = lexer.next_token();

                if (content_token == tk::BlankLine) {
                    continue;
                }

                if (content_token != tk::RightBrace) {
                    test_info.TestContent.push_back(content_token);
                } else {
                    terminator_found = true;
                }
            }
        }
        // Grab expected output
        else if (tk == tk::Identifier && txt::equal_icase(tk.text(), "expected_output")) {
            lexer.require(tk::Equal);
            lexer.require(tk::LeftBrace);

            bool terminator_found = false;
            while (!terminator_found) {
                Token content_token = lexer.next_token();

                if (content_token == tk::BlankLine) {
                    continue;
                }

                if (content_token != tk::RightBrace) {
                    content_token.Kind = str_to_token_kind(content_token.text());
                    test_info.ExpectedOutput.push_back(content_token);
                } else {
                    terminator_found = true;
                }
            }
        }
    }

    return test_info;
}

void run_test(const fs::path& test_file) {
    TestInfo test_info = parse_test_info(test_file);

    if (test_info.TestType.empty()) {
        INFO("TestType is not defined for {}; Skipping", test_file.filename().string());
        return;
    }

    INFO("Running Test - {:>25.25} :: {:<}", test_file.filename().string(), test_info.TestType);

    if (test_info.TestContent.size() != test_info.ExpectedOutput.size()) {
        std::stringstream ss{};
        ss << "Test content size differs from expected output";
        ss << "\n  Expected: " << test_info.ExpectedOutput.size();
        ss << "\n  Actual  : " << test_info.TestContent.size();
        throw std::runtime_error{ss.str().c_str()};
    }

    for (size_t i = 0; i < test_info.TestContent.size(); ++i) {
        Token token = test_info.TestContent.at(i);
        Token expected_token = test_info.ExpectedOutput.at(i);

        bool failed = (token != expected_token);

        if (expected_token.is_identifier()) {
            failed = !token.is_identifier();
        }

        if (failed) {
            std::stringstream ss{};
            ss << "Failed to match tokens";
            ss << "\n  Expected: " << str{expected_token.text()};
            ss << "\n  Actual  : " << str{token.text()} << '(' << str{token.token_name()} << ')';
            throw std::runtime_error{ss.str().c_str()};
        }
    }
}

void run_all_tests(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator{directory}) {
        if (entry.is_regular_file()) {
            try {
                run_test(entry.path());
            } catch (const std::exception& err) {
                INFO("Error running test {}", entry.path().string());
                INFO("{}", err.what());
            }
        }
    }
}

}  // namespace

}  // namespace tm_parse::tests

int main() {
    using namespace tm_parse;
    tests::run_all_tests(fs::current_path() / "data");

    TRACE("Hello World");
    INFO("Hello World");
    WARN("Hello World");
    ERR("Hello World");
}