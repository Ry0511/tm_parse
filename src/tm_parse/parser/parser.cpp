//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser.h"

#include "tm_parse/parser/rules/dot_identifier.h"

namespace tm_parse {

namespace {

str read_file(const fs::path& file) {
    using It = std::istream_iterator<str_char>;
    str_ifstream ifs{file};

    if (!ifs.is_open()) {
        throw std::runtime_error{"failed to open file"};
    }

    return str{It{ifs}, It{}};
}

}  // namespace

Parser::Parser(str text) : m_Text(std::move(text)), m_Lexer(m_Text) {}

Parser::Parser(const fs::path& file) : m_Text(read_file(file)), m_Lexer(m_Text) {}

std::unique_ptr<ParserRule> Parser::parse() {
    if (auto res = rules::DotIdentifier::create(*this)) {
        return res;
    }
    return nullptr;
}

}  // namespace tm_parse