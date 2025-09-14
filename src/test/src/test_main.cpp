//
// Date       : 11/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/util/text_helpers.h"

#include "test_file.h"

namespace tm_parse::tests {

namespace {

void run_test(const fs::path& test_file) {
    TestFile test{test_file};

    const str& test_type = test.test_type();
    INFO("Running test {} of type {}", test_file.filename().string(), test_type);

    const str& desc = test.get<const str&>("description", str{});
    if (!desc.empty()) {
        INFO(" - {}", desc);
    }

    bool is_skip_mode = test.get<bool>("skip_tokens", true);

    using Vec = std::vector<Token>;
    const auto& test_content = test.get<const Vec&>("test_content", Vec{});
    const auto& expected_output = test.get<const Vec&>("expected_output", Vec{});

    if (test_content.empty() || expected_output.empty()) {
        throw std::runtime_error{"test_content or expected_output is empty"};
    }

    size_t len = std::min(test_content.size(), expected_output.size());
    if (test_content.size() != expected_output.size()) {
        WARN("Test content size differs from expected output; Only comparing to smallest length.");
    }

    std::vector<str> errors{};

    for (size_t i = 0; i < len; ++i) {
        Token actual = test_content[i];

        Token expected = expected_output[i];
        expected.Kind = str_to_token_kind(expected.text());

        if (actual == expected || (actual.is_identifier() && expected.is_identifier())) {
            continue;
        }

        errors.emplace_back(
            std::format(
                "Test content mismatch at index {};"
                "\n Expected: {}"
                "\n Actual  : {}, {}"
                "\nAt position {}:{}",
                i,
                expected.token_name(),
                actual.token_name(),
                txt::escape_string(actual.text()),
                actual.Line,
                actual.Column
            )
        );
    }

    if (errors.empty()) {
        return;
    }

    str_stream ss{};
    for (size_t i = 0; i < errors.size(); ++i) {
        ss << errors[i];

        if (i != errors.size() - 1) {
            ss << "\n\n";
        } else {
            ss << "\n";
        }
    }

    throw std::runtime_error{ss.str()};
}

void run_all_tests(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator{directory}) {
        if (entry.is_regular_file()) {
            try {
                run_test(entry.path());
            } catch (const std::exception& err) {
                INFO("Error running test {}", entry.path().string());
                INFO("With message: {}", err.what());
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