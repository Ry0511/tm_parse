//
// Date       : 13/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "parser_test_runner.h"
#include "test_file.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/dot_identifier.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse::tests {

using namespace tm_parse::rules;

bool ParserTestRunner::run(TestFile& file) {
    // TODO: This is a first pass implementation needs to be revised a bit but generally seems like
    //  it will work. Already caught some bugs such as with PropertyAccess not extending full text
    //  to include the array if present.

    m_Success = true;
    auto text = file.get<str_view>("test_content_str");
    auto expected_rules = file.get<std::vector<ParserTestEntry>>("expected_parse_content");
    auto expected_text = file.get<TokenVec>("expected_text");

    Parser parser{str{text}};
    size_t pos = 0;

    for (const auto& expected : expected_rules) {
        {
            Matcher m = parser.create_matcher();
            if (m.maybe_real(tk::EndOfInput)) {
                break;
            }
        }

        const RuleFactory& factory = RuleTestApi::get_factory(expected.Class.text());
        info("* RuleFactory of {}", expected.Class.text());

        Matcher m = parser.create_matcher();
        if (!factory.matches(m)) {
            err("* {}::matches check failed", expected.Class.text());
            m_Success = false;
            continue;
        }

        auto rule = factory.create(parser);
        info("* Rule='{}'", txt::escape_string(rule->full_text(), true).substr(0, 50));

        if (!rule) {
            err("* {}::create check failed", expected.Class.text());
        }

        str left = txt::escape_string(rule->full_text());
        str right = txt::escape_string(expected_text[pos].inner_text());
        ++pos;

        if (left != right) {
            err("* '{}' != '{}'", left, right);
            m_Success = false;
        }

        // TODO: The general idea is implemented but this needs to be cleaned up and extracted out
        //  to separate functions.
        if (expected.VisitorTree.empty()) {
            continue;
        }

        bool all_matching = true;
        bool skip_first = true;
        size_t index = 0;
        size_t checked_rules = 0;
        rule->visit(
            [this, &skip_first, &checked_rules, &all_matching, &expected, &index](
                const auto& rule
            ) -> void {
                if (skip_first) {
                    skip_first = false;
                    return;
                }

                if (index >= expected.VisitorTree.size()) {
                    m_Success = false;
                    return;
                }

                ++checked_rules;
                const auto expected_rule = expected.VisitorTree[index].text();
                ++index;

                if (rule.rule_name() != expected_rule) {
                    all_matching = false;
                    err("* {} != {}", rule.rule_name(), expected_rule);
                }
            }
        );

        if (!all_matching) {
            err("* not all rules rules matched as expected");
            m_Success = false;
        }

        if (checked_rules != expected.VisitorTree.size()) {
            err("* expected {} nodes in parse result but got {}",
                expected.VisitorTree.size(),
                checked_rules);
            m_Success = false;
        }
    }

    return success();
}

void ParserTestRunner::assert_expected_text(TestFile& file) {}

void ParserTestRunner::assert_expected_parse_tree(TestFile& file) {}

}  // namespace tm_parse::tests
