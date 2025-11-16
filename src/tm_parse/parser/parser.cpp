//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/rules/primary/program.h"

namespace tm_parse {

namespace {

std::vector<Token> read_all_tokens(str_view text) {
    std::vector<Token> tokens{};
    tokens.reserve(512);
    Lexer lexer{text};

    do {
        tokens.emplace_back(lexer.next_token());
    } while (!lexer.is_eof());

    // ensure the last token is an EndOfInput token
    Token last = tokens.back();
    last.Kind = tk::EndOfInput;
    tokens.emplace_back(last);

    tokens.shrink_to_fit();

    return tokens;
}

}  // namespace

Parser::Parser(str text) : m_Text(std::move(text)), m_Tokens(read_all_tokens(m_Text)) {
    Matcher::set_tokens(m_Tokens);
}

std::unique_ptr<ParserRule> Parser::parse() {
    return create<rules::ProgramRule>();
}

}  // namespace tm_parse