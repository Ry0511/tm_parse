//
// Date       : 27/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/lexer/token_error.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse {

Lexer::Lexer(str_view source) : m_Text(source) {}

Token Lexer::next_token() {
    return next_token_impl();
}

Token Lexer::next_real_token() {
    Token tk{};

    do {
        tk = next_token_impl();

        switch (tk.Kind) {
            case tk::BlankLine:
            case tk::LineComment:
            case tk::MultiLineComment:
                continue;
            default:
                return tk;
        }

    } while (tk != tk::EndOfInput);

    return Token{};
}

Token Lexer::peek_token() {
    Lexer state = save_state();
    Token token = next_token_impl();
    restore_state(state);
    return token;
}

void Lexer::skip_whitespace() {
    while (!is_eof() && txt::is_whitespace(peek())) {
        m_Column++;
        m_Pos++;
    }
}

str_char Lexer::peek() const {
    if (is_eof()) {
        return TXT('\0');
    }
    return m_Text[m_Pos];
}

str_char Lexer::peek(int offset) const {
    if (is_eof()) {
        return TXT('\0');
    }
    auto abs_offset = (offset < 0) ? static_cast<size_t>(-offset) : static_cast<size_t>(offset);

    // Lookbehind
    if (offset < 0) {
        if (abs_offset > m_Pos) {
            return TXT('\0');
        }
        return m_Text[m_Pos - abs_offset];
    }

    // Lookahead
    if ((m_Pos + abs_offset) >= m_Text.size()) {
        return TXT('\0');
    }
    return m_Text[m_Pos + abs_offset];
}

str_char Lexer::advance() {
    if (is_eof()) {
        return TXT('\0');
    }

    const str_char c = m_Text[m_Pos];
    m_Pos++;

    if (c == TXT('\n')) {
        m_Line++;
        m_Column = 0;
    }
    m_Column++;

    return c;
}

Token Lexer::create_token(tk::TokenKind kind) {
    if (m_Pos < m_Start) {
        throw std::logic_error{"invalid lexer state"};
    }

    return Token{
        kind,
        TextRegion{m_Start, m_Pos},
        m_Line,
        static_cast<int>(static_cast<size_t>(m_Column) - (m_Pos - m_Start)),
        m_Text.data(),
    };
}

Token Lexer::next_token_impl() {
    // Skip to next valid text input
    skip_whitespace();

    // Check for end of input
    if (is_eof()) {
        return Token{};
    }

    // [\r\n]+
    if (txt::is_newline(peek())) {
        return read_blankline();
    }

    // /* ... */
    if (peek() == TXT('/') && peek(1) == TXT('*')) {
        return read_multiline_comment();
    }

    // # [^\n]+
    if (peek() == TXT('#')) {
        return read_line_comment();
    }

    // "[^"\n]*" with \" being used to escape quotes
    if (peek() == TXT('"')) {
        return read_string_literal();
    }

    // [+-]?\d+(\.\d+)?
    bool has_prefix = (peek() == TXT('-') || peek() == TXT('+'));
    if (txt::is_digit(peek()) || (has_prefix && txt::is_digit(peek(1)))) {
        return read_number();
    }

    // [a-zA-Z_][a-zA-Z0-9_]*
    if (txt::is_alpha(peek()) || peek() == TXT('_')) {
        return read_identifier();
    }

    // Capture all token
    return read_other();
}

////////////////////////////////////////////////////////////////////////////////
// | UTILITY FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

Token Lexer::require(tk::TokenKind kind) noexcept(false) {
    Lexer state = save_state();
    Token tk = next_token();

    if (tk != kind) {
        restore_state(state);
        throw TokenError{std::format("Expecting token of type {}", token_type_name(kind)), tk};
    }

    return tk;
}

Token Lexer::require_next_real(tk::TokenKind kind) noexcept(false) {
    Lexer state = save_state();
    Token tk = next_real_token();

    if (tk != kind) {
        restore_state(state);
        throw TokenError{std::format("Expecting token of type {}", token_type_name(kind)), tk};
    }

    return tk;
}

Token Lexer::maybe(tk::TokenKind kind) noexcept {
    Lexer state = save_state();
    Token tk = next_token();

    if (tk != kind) {
        restore_state(state);
        return Token{tk::InvalidToken};
    }

    return tk;
}

Token Lexer::maybe_next_real(tk::TokenKind kind) noexcept {
    Lexer state = save_state();
    Token tk = next_real_token();

    if (tk != kind) {
        restore_state(state);
        return Token{tk::InvalidToken};
    }

    return tk;
}

////////////////////////////////////////////////////////////////////////////////
// | READ FUNCTIONS |
////////////////////////////////////////////////////////////////////////////////

Token Lexer::read_blankline() {
    m_Start = m_Pos;

    if (peek() == TXT('\r')) {
        advance();
    }

    if (peek() == TXT('\n')) {
        advance();
    }

    return create_token(tk::BlankLine);
}

Token Lexer::read_identifier() {
    m_Start = m_Pos;  // Start token

    advance();  // Skip first char

    // [a-zA-Z_][a-zA-Z0-9_]+
    while (txt::is_identifier(peek())) {
        advance();
    }

    Token tk = create_token(tk::Identifier);

    // If the length of the token content is within the bounds of a keyword see if it matches any
    // known keyword tokens
    constinit static size_t min_len = smallest_keyword_length();
    constinit static size_t max_len = largest_keyword_length();
    size_t len = tk.Region.length();

    if (len >= min_len && len <= max_len) {
        str_view text = tk.Region.create_str_view(m_Text);

        for (size_t i = token_type_kw_start; i < token_type_kw_end; ++i) {
            auto kind = static_cast<tk::TokenKind>(i);
            if (txt::equal_icase(token_type_name(kind), text)) {
                tk.Kind = kind;
                return tk;
            }
        }
    }

    return tk;
}

Token Lexer::read_number() {
    m_Start = m_Pos;  // Start token

    if (peek() == TXT('-') || peek() == TXT('+')) {
        advance();
    }

    // Consume digits
    while (txt::is_digit(peek())) {
        advance();
    }

    // Consume \.\d+
    if (peek(0) == TXT('.') && txt::is_digit(peek(1))) {
        advance();

        while (txt::is_digit(peek())) {
            advance();
        }
    }

    return create_token(tk::Number);
}

Token Lexer::read_other() {
    auto _create_token = [&](tk::TokenKind kind) {
        m_Start = m_Pos;
        m_Pos++;
        return create_token(kind);
    };

    // clang-format off
    switch (peek()) {
        case TXT('['):  return _create_token(tk::LeftBracket);
        case TXT(']'):  return _create_token(tk::RightBracket);
        case TXT('('):  return _create_token(tk::LeftParen);
        case TXT(')'):  return _create_token(tk::RightParen);
        case TXT('.'):  return _create_token(tk::Dot);
        case TXT(':'):  return _create_token(tk::Colon);
        case TXT('/'):  return _create_token(tk::Slash);
        case TXT('*'):  return _create_token(tk::Star);
        case TXT('+'):  return _create_token(tk::Plus);
        case TXT('-'):  return _create_token(tk::Minus);
        case TXT(','):  return _create_token(tk::Comma);
        case TXT('='):  return _create_token(tk::Equal);
        case TXT('\''): return _create_token(tk::SingleQuote);
        case TXT('!'):  return _create_token(tk::ExclamationMark);
        case TXT('?'):  return _create_token(tk::QuestionMark);
        case TXT('$'):  return _create_token(tk::DollarSign);
        case TXT('{'):  return _create_token(tk::LeftBrace);
        case TXT('}'):  return _create_token(tk::RightBrace);
        case TXT('&'):  return _create_token(tk::Ampersand);
        default:        return _create_token(tk::OtherText);
    }
    // clang-format on
}

Token Lexer::read_line_comment() {
    m_Start = m_Pos;
    m_Pos++;
    while (!txt::is_newline(peek())) {
        advance();
    }
    return create_token(tk::LineComment);
}

Token Lexer::read_multiline_comment() {
    Lexer state = save_state();

    m_Start = m_Pos;
    m_Pos += 2;

    bool terminator_found = false;

    // Consume until we reach the terminating sequence */ or the end of the input
    while (!is_eof() && !terminator_found) {
        terminator_found = (peek() == TXT('*') && peek(1) == TXT('/'));
        advance();
    }

    // Consumed the entire stream just restore the lexer state and process as OtherText
    if (is_eof() && !terminator_found) {
        restore_state(state);
        return read_other();
    }

    m_Pos++;  // peek() == '/'
    return create_token(tk::MultiLineComment);
}

Token Lexer::read_string_literal() {
    Lexer state = save_state();

    m_Start = m_Pos;
    m_Pos++;

    bool terminator_found = false;

    // Consume until we reach the terminating sequence " or the end of the input
    while (!is_eof() && !terminator_found) {
        str_char c = advance();
        // peek(-1) == c
        terminator_found = (c == TXT('\"')) && peek(-2) != TXT('\\');

        if (txt::is_newline(c)) {
            restore_state(state);
            return read_other();
        }
    }

    // Consumed the entire stream just restore the lexer state and process as OtherText
    if (is_eof() && !terminator_found) {
        restore_state(state);
        return read_other();
    }

    // m_Pos++;  // peek() == "
    return create_token(tk::StringLiteral);
}

}  // namespace tm_parse