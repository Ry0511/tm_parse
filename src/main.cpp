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
#include "tm_parse/parser/rules.h"
#include "tm_parse/gen/asm_generator.h"

using namespace tm_parse;

int main() {

    str source = TXT(R"(
      create_mod = (
        name            = "My Mod Name",
        version         = (1, 0),
        authors         = ((name="-Ry"), (name="Other")),
        supported_games = ("BL1", "BL2", "TPS"),
        description     = "this is an example mod",
        license         = (name = "GPL3", url = "https://choosealicense.com/licenses/gpl-3.0/"),
        float_number    = 3.14159265358979323846264338327950
      )
    )");

    Parser parser{source};
    auto prog = parser.parse();
    gen::AsmGenerator generator{};
    generator.evaluate(prog->as_ref<rules::ProgramRule>(), GeneratorContext{});

    LOG_INFO("Parser created with string: {}", source);
}