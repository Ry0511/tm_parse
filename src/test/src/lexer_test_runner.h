//
// Date       : 14/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "test_runner.h"

namespace tm_parse::tests {

class LexerTestRunner : public TestRunner {
   public:
    LexerTestRunner() = default;
    ~LexerTestRunner() override = default;

   public:
    bool run(TestFile& file) override;

   private:
    void assert_expected_tokens(TestFile& file);
    void assert_expected_text(TestFile& file);
};

}  // namespace tm_parse::tests
