//
// Date       : 01/10/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token_error.h"
#include "tm_parse/parser/matcher.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse {

Matcher::Matcher() noexcept = default;

Matcher::Matcher(const Parser& parser) noexcept
    : m_Tokens(parser.m_Tokens),
      m_Position(parser.position()) {}

Matcher::Matcher(Parser& parser) noexcept
    : m_Tokens(parser.m_Tokens),
      m_Position(parser.position()) {}

Matcher::Matcher(std::span<const Token> tokens, size_t pos) noexcept
    : m_Tokens(tokens),
      m_Position(pos) {}

bool Matcher::is_eof() const noexcept {
    const Token& tok = peek_real();
    return !tok || tok.is_eof();
}

bool Matcher::try_match(std::span<const tk::TokenKind> kinds) noexcept {
    auto pos = position();

    bool res = std::ranges::all_of(kinds, [this](const auto& kind) -> bool {
        return this->next() == kind;
    });

    if (!res) {
        m_Position = pos;
        return false;
    }

    return true;
}

bool Matcher::try_match_real(std::span<const tk::TokenKind> kinds) noexcept {
    auto pos = position();

    bool res = std::ranges::all_of(kinds, [this](const auto& kind) -> bool {
        return this->next_real() == kind;
    });

    if (!res) {
        m_Position = pos;
        return false;
    }

    return true;
}

MatcherErrorInfo Matcher::get_context_range(size_t max_line_tokens) noexcept {
    size_t begin = std::min(m_Position, m_Tokens.size() - 1);
    size_t end = begin;
    size_t count = max_line_tokens;

    // Move 'begin' back N lines or to the start of the input
    while (count > 0 && begin > 0) {
        if (m_Tokens[begin] == tk::BlankLine) {
            --count;
            if (count <= 0) {
                break;
            }
        }
        --begin;
    }

    // m_Position inside the token range; effectively the last 'valid' token
    size_t last_valid = (end == begin) ? 0 : ((end - begin) - 1);

    // Move 'end' to the end of the line or end of input
    while (end < m_Tokens.size() && m_Tokens[end] != tk::BlankLine) {
        ++end;
    }

    return MatcherErrorInfo{last_valid, m_Tokens.subspan(begin, end - begin)};
}

str Matcher::get_error_string(str_view expected) noexcept {
    const auto info = get_context_range();
    str_stream ss{};

    if (info.empty()) {
        ss << TXT("Expected ") << expected;
        if (m_Tokens.empty()) {
            ss << TXT(" but the stream was empty");
        } else {
            const Token& next = peek_real();
            ss << TXT(" but got ") << next.token_name();
        }
        return ss.str();
    }

    const Token& last_valid = info.last_valid();
    const Token& first = info.front();
    const Token& last = info.back();
    TextRegion region = first.Region.extend(last.Region);

    if (last_valid.is_eof()) {
        ss << TXT("Expected ") << expected << TXT(" but got EndOfInput");
        return ss.str();
    }

    ss << TXT('\n');

    // Build the content string
    constexpr str_view indent = TXT(" * ");
    str_stream content{region.create_str(first.Text)};
    str line{};
    while (std::getline(content, line)) {
        if (line.empty() || line.find_first_not_of(TXT(' ')) > line.length()) {
            continue;
        }
        ss << indent << line << TXT('\n');
    }

    // Position indicator
    auto len = last_valid.Region.length();
    if (len > 0) {
        auto column = static_cast<size_t>(last_valid.Column) - 1;
        ss << str(column + indent.size(), TXT(' ')) << str(len, TXT('^')) << TXT('\n');
    }

    ss << std::format("Expected {} after {}", expected, last_valid.text());
    return ss.str();
}

const Token& Matcher::peek() const noexcept {
    if (position() >= m_Tokens.size()) {
        return invalid_token_v;
    }
    return m_Tokens[m_Position];
}

const Token& Matcher::peek_real() const noexcept {
    size_t pos = m_Position;

    while (pos < m_Tokens.size()) {
        const Token& tok = m_Tokens[pos++];
        switch (tok.Kind) {
            case tk::LineComment:
            case tk::MultiLineComment:
            case tk::BlankLine:
                continue;
            default:
                return tok;
        }
    }

    return invalid_token_v;
}

const Token& Matcher::next() noexcept {
    if (position() >= m_Tokens.size()) {
        return invalid_token_v;
    }
    return m_Tokens[m_Position++];
}

const Token& Matcher::next_real() noexcept {
    do {
        const Token& tk = this->next();

        switch (tk.Kind) {
            case tk::LineComment:
            case tk::MultiLineComment:
            case tk::BlankLine:
                continue;
            default:
                return tk;
        }

    } while (!is_eof());

    return invalid_token_v;
}

const Token& Matcher::maybe(tk::TokenKind kind) noexcept {
    size_t pos = m_Position;
    const Token& tk = next();

    if (tk == kind) {
        return tk;
    }

    m_Position = pos;
    return invalid_token_v;
}

const Token& Matcher::maybe_real(tk::TokenKind kind) noexcept {
    size_t pos = m_Position;
    const Token& tk = next_real();

    if (tk == kind) {
        return tk;
    }

    m_Position = pos;
    return invalid_token_v;
}

const Token& Matcher::any(const std::span<const tk::TokenKind>& kinds) noexcept {
    size_t pos = m_Position;
    const Token& tk = next();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return invalid_token_v;
}

const Token& Matcher::any_real(const std::span<const tk::TokenKind>& kinds) noexcept {
    size_t pos = m_Position;
    const Token& tk = next_real();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return invalid_token_v;
}

const Token& Matcher::not_any(const std::span<const tk::TokenKind>& kinds) noexcept {
    size_t pos = m_Position;
    const Token& tk = next();

    for (const auto& kind : kinds) {
        if (tk == kind) {
            return tk;
        }
    }

    m_Position = pos;
    return invalid_token_v;
}

const Token& Matcher::require(tk::TokenKind kind, const SrcLoc& src) {
    size_t pos = m_Position;
    const Token& tk = next();
    if (tk != kind) {
        m_Position = pos;
        throw TokenError{get_error_string(token_type_name(kind)), tk, src};
    }
    return tk;
}

const Token& Matcher::require_real(tk::TokenKind kind, const SrcLoc& src) {
    size_t pos = m_Position;
    const Token& tk = next_real();

    if (tk != kind) {
        m_Position = pos;
        throw TokenError{get_error_string(token_type_name(kind)), tk, src};
    }

    return tk;
}

}  // namespace tm_parse
