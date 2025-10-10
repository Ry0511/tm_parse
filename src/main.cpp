//
// Date       : 26/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/util/text_helpers.h"

#include "tm_parse/parser/rules/expr/composed_expr.h"

using namespace tm_parse;

int main() {
    // str source = TXT(R"(
    //     set
    //       foo.baz:bar
    //       property.bar(1)
    //       ( A = ( X= 10, Y=20 )
    //       , B = ( Z=-10, W=3  )
    //       , C = ( D=(A=FALSE,B=TRUE,C=True,D=False) )
    //       )
    // )");

    str source = TXT(R"(
      Begin Object Class=SomeClass Name=SomeName

        A = ( A * B / -( C / D + 5.0 ) )
        B = "Swear" / "Word" * "uh oh"

      End Object
    )");

    Parser parser{source};
    using namespace rules;

    try {
        do {
            auto ptr = parser.parse();
            ptr->cascade_assign_parents(nullptr);

            ptr->visit([](const auto& node) -> void {
                int depth = node.get_depth() * 2;
                str indent(depth, TXT(' '));

                int len = std::max(0, 36 - depth);

                LOG_INFO(
                    "{}{:<{}} ~ {}",
                    indent,
                    node.rule_name(),
                    len,
                    txt::escape_string(node.full_text()).substr(0, 50)
                );
            });

            LOG_INFO(
                "ParsedRule ~ {} from '{}'",
                ptr->rule_name(),
                txt::escape_string(ptr->full_text(), true)
            );

        } while (!parser.is_eof());

    } catch (const TokenError& err) {
        err.log_error();
    }

    LOG_INFO("Parser created with string: {}", source);
}