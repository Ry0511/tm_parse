//
// Date       : 27/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse {

Lexer::Lexer(str_view source) : m_Text(source) {}

Token Lexer::next_token() {
    return next_token_impl();
}

Token Lexer::peek_token() {
    Lexer state = save_state();
    Token token = next_token_impl();
    restore_state(state);
    return token;
}

void Lexer::skip_whitespace() {
    // Skip the current character if and only if it is a whitespace character [\n\r\t\v ]+
    while (!is_eof() && txt::is_whitespace(peek())) {
        advance();
    }
}

char Lexer::peek() const {
    if (is_eof()) {
        throw std::runtime_error{"peek out of bounds"};
    }
    return m_Text[m_Pos];
}

char Lexer::peek(int offset) const {
    if (is_eof()) {
        throw std::runtime_error{"peek out of bounds; eof reached"};
    }
    auto abs_offset = (offset < 0) ? static_cast<size_t>(-offset) : static_cast<size_t>(offset);

    // Lookbehind
    if (offset < 0) {
        if (abs_offset > m_Pos) {
            throw std::runtime_error{"peek out of bounds; lookbehind"};
        }
        return m_Text[m_Pos - abs_offset];
    }

    // Lookahead
    if ((m_Pos + abs_offset) >= m_Text.size()) {
        throw std::runtime_error{"peek out of bounds; lookahead"};
    }
    return m_Text[m_Pos + abs_offset];
}

char Lexer::advance() {
    if (is_eof()) {
        throw std::runtime_error{"advance out of bounds; eof reached"};
    }
    const char c = m_Text[m_Pos];
    m_Pos++;

    if (c == TXT('\n')) {
        m_Line++;
    }

    return c;
}

Token Lexer::create_token(tk::TokenKind kind) {
    return Token{
        kind,
        TextRegion{m_Start, m_Pos},
    };
}

Token Lexer::next_token_impl() {
    // Skip to next valid text input
    skip_whitespace();

    // Check for end of input
    if (is_eof()) {
        return Token{};
    }

    while (!txt::is_digit(peek())) {
        advance();

        if (is_eof()) {
            return Token{};  // Default EOF token
        }
    }

    m_Start = m_Pos;
    advance();
    Token tk = create_token(tk::TokenKind::Number);
    return tk;
}

}  // namespace tm_parse