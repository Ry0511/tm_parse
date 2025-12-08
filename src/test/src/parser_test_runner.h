//
// Date       : 13/10/2025
// Project    : tm_parse
// Author     : -Ry
//
#pragma once

#include "tm_parse/pch.h"
#include "test_runner.h"

namespace tm_parse::tests {

class ParserTestRunner : public TestRunner {
   public:
    ParserTestRunner() = default;
    ~ParserTestRunner() override = default;

   public:
    bool run(TestFile& file) override;

   private:
    void assert_expected_text(TestFile& file);
    void assert_expected_parse_tree(TestFile& file);
};

}  // namespace tm_parse::tests
