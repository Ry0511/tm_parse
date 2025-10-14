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

        const RuleFactory& factory = RuleTestApi::get_factory(expected.Class);

        Matcher m = parser.create_matcher();
        if (!factory.matches(m)) {
            err("* {}::matches check failed", expected.Class);
            m_Success = false;
            continue;
        }

        auto rule = factory.create(parser);

        if (!rule) {
            err("* {}::create check failed", expected.Class);
        }

        str left = txt::escape_string(rule->full_text());
        str right = txt::escape_string(expected_text[pos].inner_text());
        ++pos;

        if (left != right) {
            err("* {} != {}", left, right);
            m_Success = false;
        }

        // TODO: The general idea is implemented but this needs to be cleaned up and extracted out
        //  to separate functions.
        if (expected.VisitorTree.empty()) {
            continue;
        }

        struct {
            bool First{true};
            size_t Index{0};
        } state;

        rule->visit([this, &state, &expected](const auto& rule) -> void {
            if (state.Index >= expected.VisitorTree.size()) {
                return;
            }

            // skip entry rule
            if (state.First) {
                state.First = false;
                return;
            }

            str_view rule_name = rule.rule_name();
            str rule_text = txt::escape_string(rule.full_text());
            const auto& [name, text] = expected.VisitorTree.at(state.Index);
            ++state.Index;

            if (rule_name != name) {
                info("* {} <> {}", rule.rule_name(), rule.full_text());
                err("* {} != {}", rule_name, name);
                m_Success = false;
            }

            if (!text.empty() && rule_text != text) {
                info("* {} <> {}", rule.rule_name(), rule.full_text());
                err("* {} != {}", rule_text, text);
                m_Success = false;
            }
        });
    }

    return success();
}

void ParserTestRunner::assert_expected_text(TestFile& file) {}

void ParserTestRunner::assert_expected_parse_tree(TestFile& file) {}

}  // namespace tm_parse::tests
