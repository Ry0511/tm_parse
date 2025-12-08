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
#include "tm_parse/parser/rules/primary/object_definition.h"

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

            ptr->visit([](const ParserRule& node) -> void {

                int depth = node.get_depth() * 2;
                int len = std::max(0, 36 - depth);
                str indent(depth, TXT(' '));
                str suffix{};

                if (const auto* ptr = node.is<BinaryOpExpr>()) {
                    // clang-format off
                    switch (ptr->op()) {
                        case Operator::Add:      { suffix += TXT(" + "); break; }
                        case Operator::Subtract: { suffix += TXT(" - "); break; }
                        case Operator::Divide:   { suffix += TXT(" / "); break; }
                        case Operator::Multiply: { suffix += TXT(" * "); break; }
                        default:                 { suffix += TXT(" ? "); break; }
                    }
                    // clang-format on

                    LOG_INFO(
                        "{}{:<{}} ~ ({}){}({})",
                        indent,
                        str{node.rule_name()} + suffix,
                        len,
                        txt::escape_string(ptr->left().full_text()).substr(0, 50),
                        suffix,
                        txt::escape_string(ptr->right().full_text()).substr(0, 50)
                    );

                } else {
                    LOG_INFO(
                        "{}{:<{}} ~ {}",
                        indent,
                        str{node.rule_name()} + suffix,
                        len,
                        txt::escape_string(node.full_text()).substr(0, 50)
                    );
                }
            });

            LOG_INFO(
                "ParsedRule ~ {} from '{}'",
                ptr->rule_name(),
                txt::escape_string(ptr->full_text(), true)
            );

            // TODO: Fix
            if (parser.maybe_real(tk::EndOfInput)) {
                break;
            }

        } while (!parser.is_eof());

    } catch (const TokenError& err) {
        err.log_error();
    }

    LOG_INFO("Parser created with string: {}", source);
}