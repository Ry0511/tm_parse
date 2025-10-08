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

        Begin Object Class=Child Name=Child_00
          A=(B=10, C=20, D="Some String")
        End Object

        B="My String"
        C=(B=10, C=20, D="Some String", E=(X = 10, Y = 20, Z = (W = "String!")))
        D=(E=$(my_option.value), F=$(wpc.Pawn.Location))

        E = $(SYS.Globals)

      End Object
    )");

    Parser parser{source};

    try {
        do {
            auto ptr = parser.parse();

            ptr->visit([](const auto& node) -> void {
                int depth = node.get_depth() * 2;
                str indent(depth, TXT(' '));
                LOG_INFO("{}{}", indent, node.rule_name());
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