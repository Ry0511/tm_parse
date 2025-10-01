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
    str source = TXT(R"(
        set
          foo.baz:bar
          property.bar
          ( A = ( X= 10, Y=20 )
          , B = ( Z=-10, W=3  )
          )
    )");

    Parser parser{source};

    try {
        do {
            auto ptr = parser.parse();

            // TODO: A way to gauge depth would be nice here
            ptr->visit([](const auto& node) -> void {
                LOG_INFO("{} => '{}'", node.rule_name(), txt::escape_string(node.full_text()));
            });

            LOG_INFO(
                "ParsedRule ~ {} from '{}'",
                ptr->rule_name(),
                txt::escape_string(ptr->full_text())
            );

        } while (!parser.is_eof());

    } catch (const TokenError& err) {
        err.log_error();
    }

    LOG_INFO("Parser created with string: {}", source);
}