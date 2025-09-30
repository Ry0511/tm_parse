//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/rules/set_command.h"

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

std::vector<Token> read_all_tokens(Lexer& lexer) {
    std::vector<Token> tokens{};
    tokens.reserve(512);

    do {
        tokens.emplace_back(lexer.next_token());
    } while (!lexer.is_eof());

    return tokens;
}

}  // namespace

Parser::Parser(str text)
    : m_Text(std::move(text)),
      m_Lexer(m_Text),
      m_Tokens(read_all_tokens(m_Lexer)) {}

Parser::Parser(const fs::path& file) : m_Text(read_file(file)), m_Lexer(m_Text) {}

std::unique_ptr<ParserRule> Parser::parse() {
    return rules::SetCommand::create(*this);
}

Token Parser::peek(int offset) const noexcept {
    // Lookbehind
    if (offset < 0) {
        auto poffset = static_cast<size_t>(offset * -1);
        size_t index = position();

        if (index > poffset) {
            return m_Tokens.at(index - poffset);
        }

        return Token{tk::InvalidToken};
    }

    // Lookahead
    size_t index = position() + static_cast<size_t>(offset);
    if (index < m_Tokens.size()) {
        return m_Tokens.at(index);
    }

    return Token{tk::InvalidToken};
}

Token Parser::next() noexcept {
    if (m_Position >= m_Tokens.size()) {
        return Token{};
    }
    return m_Tokens[m_Position++];
}

Token Parser::next_real() noexcept {
    Token tk{};

    do {
        tk = this->next();

        switch (tk.Kind) {
            case tk::BlankLine:
            case tk::LineComment:
            case tk::MultiLineComment:
                continue;
            default:
                return tk;
        }

    } while (tk != tk::EndOfInput);

    return tk;
}

Token Parser::require(tk::TokenKind kind) {
    size_t saved_pos = m_Position;
    Token tk = this->next();

    if (tk != kind) {
        m_Position = saved_pos;
        throw TokenError{std::format("expecting '{}'", token_type_name(kind)), tk};
    }

    return tk;
}

Token Parser::require_next_real(tk::TokenKind kind) {
    size_t saved_pos = m_Position;
    Token tk = this->next_real();

    if (tk != kind) {
        m_Position = saved_pos;
        throw TokenError{std::format("expecting '{}'", token_type_name(kind)), tk};
    }

    return tk;
}

Token Parser::maybe(tk::TokenKind kind) {
    size_t saved_pos = m_Position;
    Token tk = this->next();

    if (tk != kind) {
        m_Position = saved_pos;
        return Token{tk::InvalidToken};
    }

    return tk;
}

Token Parser::maybe_next_real(tk::TokenKind kind) {
    size_t saved_pos = m_Position;
    Token tk = this->next_real();

    if (tk != kind) {
        m_Position = saved_pos;
        return Token{tk::InvalidToken};
    }

    return tk;
}

}  // namespace tm_parse