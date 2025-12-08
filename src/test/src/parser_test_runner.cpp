//
// Date       : 13/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "parser_test_runner.h"
#include "test_file.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/common/prop_dot_identifier.h"
#include "tm_parse/util/text_helpers.h"

#include "tm_parse/parser/rules/expr/literal_expr.h"

namespace tm_parse::tests {

using namespace tm_parse::rules;

namespace {
std::unordered_set<str> missing_parent_warnings{};
}

bool ParserTestRunner::run(TestFile& file) {
    // TODO: This is a first pass implementation needs to be revised a bit but generally seems like
    //  it will work. Already caught some bugs such as with PropertyAccess not extending full text
    //  to include the array if present.

    m_Success = true;
    auto text = file.get<str_view>("test_content_str");
    auto expected_rules = file.get<std::vector<ParserTestEntry>>("expected_parse_content");
    auto expected_text = file.get<TokenVec>("expected_text");
    auto skip_blank_lines = file.get<bool>("skip_blank_lines", false);

    auto delimiter_token = tk::EndOfInput;
    str delimiter_text = file.get<str>("delimited_by", "");
    if (!delimiter_text.empty()) {
        delimiter_token = str_to_token_kind(delimiter_text);
    }

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

        if (skip_blank_lines) {
            while (m.maybe(tk::BlankLine)) {}
            while (parser.maybe(tk::BlankLine)) {}
        }

        if (!factory.matches(m)) {
            err("* {}::matches check failed", expected.Class);
            m_Success = false;
            continue;
        }

        auto rule = factory.create(parser);

        if (!rule) {
            err("* {}::create check failed", expected.Class);
        }

        if (delimiter_token != tk::EndOfInput) {
            parser.maybe_real(delimiter_token);
        }

        // TODO: we never verify the index for pos
        str left = txt::escape_string(rule->full_text());
        str right = txt::escape_string(expected_text[pos].literal_text());
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

        rule->visit([this, &rule, &state, &expected](const auto& child) -> void {
            if (state.Index >= expected.VisitorTree.size()) {
                return;
            }

            // skip entry rule
            if (state.First) {
                state.First = false;
                return;
            }

            // Sanity check to ensure no child node doesn't have a parent
            str key = str{rule->rule_name()} + str{child.rule_name()};
            const auto&[_, added] = missing_parent_warnings.insert(key);
            if (child.parent() == nullptr && added) {
                LOG_WARN(
                    "Child rule of '{}' does not set the parent for '{}'",
                    rule->rule_name(),
                    child.rule_name()
                );
            }

            str_view rule_name = child.rule_name();
            str rule_text = txt::escape_string(child.full_text());

            if (const auto* lit = child.template is<rules::LiteralExpr>()) {
                if (const Str* text = lit->template get_if<Str>()) {
                    rule_text = *text;
                }
            }
            const auto& [name, text] = expected.VisitorTree.at(state.Index);
            ++state.Index;

            info("Rule is '{}' parsed from '{}'", child.rule_name(), child.full_text());
            if (rule_name != name) {
                err("Expecting Rule '{}' but got '{}'", name, rule_name);
                m_Success = false;
            }

            if (!text.empty() && rule_text != text) {
                err("Expecting text '{}' but got '{}'", text, rule_text);
                m_Success = false;
            }
        });
    }

    return success();
}

void ParserTestRunner::assert_expected_text(TestFile& file) {}

void ParserTestRunner::assert_expected_parse_tree(TestFile& file) {}

}  // namespace tm_parse::tests
