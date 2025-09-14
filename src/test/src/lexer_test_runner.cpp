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

    using Vec = std::vector<Token>;
    const auto& test_content = test.get<const Vec&>("test_content", Vec{});
    const auto& expected_output = test.get<const Vec&>("expected_tokens", Vec{});
    const bool abort_on_first_error = test.get<bool>("abort_on_first_error", true);

    this->info("test_content size is {}; expected_output size is {}", test_content.size(), expected_output.size());

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

    return success();
}

}  // namespace tm_parse::tests