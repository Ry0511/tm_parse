//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser.h"

#include "rules/primary/object_definition.h"
#include "rules/primary/set_command.h"
#include "tm_parse/lexer/token_error.h"

namespace tm_parse {

namespace {

std::vector<Token> read_all_tokens(str_view text) {
    std::vector<Token> tokens{};
    tokens.reserve(512);
    Lexer lexer{text};

    do {
        tokens.emplace_back(lexer.next_token());
    } while (!lexer.is_eof());

    tokens.shrink_to_fit();

    return tokens;
}

}  // namespace

Parser::Parser(str text)
    : m_Text(std::move(text)),
      m_Tokens(read_all_tokens(m_Text)),
      m_Matcher(m_Tokens) {}

std::unique_ptr<ParserRule> Parser::parse() {
    Matcher m = *this;

    if (rules::SetCommand::matches(m)) {
        return rules::SetCommand::create(*this);
    }

    return rules::ObjectDefinition::create(*this);
}

}  // namespace tm_parse