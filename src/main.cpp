//
// Date       : 26/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/parser/parser.h"

using namespace tm_parse;

int main() {
    str source = TXT(R"(
        set foo.baz:bar property ( A=( X=10,Y=20 ), B = ( Z=-10, W=3) )
    )");

    Parser parser{source};

    LOG_INFO("Parser created with string: {}", source);
}