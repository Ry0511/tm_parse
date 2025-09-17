//
// Date       : 14/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "lexer_test_runner.h"

#include "test_file.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::tests {

bool LexerTestRunner::run(TestFile& test) {
    this->m_ExecutionLog.clear();
    m_Success = true;

    // Info log description if exists
    const str& desc = test.get<const str&>("description", str{});
    if (!desc.empty()) {
        this->info("- {}", desc);
    }

    assert_expected_tokens(test);

    if (success()) {
        assert_expected_text(test);
    }

    return success();
}

void LexerTestRunner::assert_expected_tokens(TestFile& test) {
    const auto& test_content = test.get<const TokenVec&>("test_content", TokenVec{});
    const auto& expected_output = test.get<const TokenVec&>("expected_tokens", TokenVec{});
    const bool abort_on_first_error = test.get<bool>("abort_on_first_error", true);

    this->info(
        "test_content size is {}; expected_output size is {}",
        test_content.size(),
        expected_output.size()
    );

    size_t len = std::min(test_content.size(), expected_output.size());
    if (test_content.size() != expected_output.size()) {
        this->info("test content size differs from expected output; using smallest of the two");
    }

    for (size_t i = 0; i < len; ++i) {
        Token actual = test_content[i];

        Token expected = expected_output[i];
        expected.Kind = str_to_token_kind(expected.text());

        if (actual == expected || (actual.is_identifier() && expected.is_identifier())) {
            continue;
        }

        this->err("* check failed : {} != {}", actual.token_name(), expected.token_name());
        this->err("* actual text  : {}", txt::escape_string(actual.text()));
        this->err("* failure at   : {}:{}", actual.Line, actual.Column);
        this->err("* expected at  : {}:{}", expected.Line, expected.Column);
        m_Success = false;

        if (abort_on_first_error) {
            break;
        }
    }
}

void LexerTestRunner::assert_expected_text(TestFile& test) {
    const auto& actual = test.get<TokenVec>("test_content", TokenVec{});
    const auto& expected = test.get<TokenVec>("expected_text", TokenVec{});
    const bool abort_on_first_error = test.get<bool>("abort_on_first_error", true);

    size_t len = std::min(actual.size(), expected.size());

    for (size_t i = 0; i < len; ++i) {
        Token actual_token = actual[i];
        Token expected_token = expected[i];

        str_view left = txt::escape_string(actual_token.inner_text());
        str_view right = txt::escape_string(expected_token.inner_text());

        if (left != right) {
            this->err("* check failed : '{}' != '{}'", left, right);
            this->err("* failure at   : {}:{}", actual_token.Line, actual_token.Column);
            this->err("* expected at  : {}:{}", expected_token.Line, expected_token.Column);
            m_Success = false;
        }

        if (abort_on_first_error && !success()) {
            break;
        }
    }
}

}  // namespace tm_parse::tests