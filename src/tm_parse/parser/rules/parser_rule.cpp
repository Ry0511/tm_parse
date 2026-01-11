//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/parser_rule.h"
#include "tm_parse/util/text_helpers.h"

namespace tm_parse {

ParserRule::~ParserRule() = default;

ParserRule* ParserRule::root() const noexcept {
    ParserRule* ptr = m_Parent;
    while (ptr && ptr->parent()) {
        ptr = ptr->parent();
    }
    return ptr;
}

str_view ParserRule::full_text() const {
    if (m_TextSource == nullptr) {
        throw std::runtime_error{"text source is null"};
    }

    return full_text_region().create_str_view(m_TextSource);
}

int ParserRule::get_depth() const noexcept {
    int depth = 0;
    const ParserRule* ptr = parent();
    while (ptr) {
        ptr = ptr->parent();
        ++depth;
    }
    return depth;
}

void ParserRule::post_init(const Token& first, const Token& last) noexcept {
    m_TextSource = first.Text;
    m_FullTextRegion = first.Region.extend(last.Region);
    m_FirstToken = first;
    m_LastToken = last;
}

void ParserRule::post_init(const Token& first) noexcept {
    m_TextSource = first.Text;
    m_FullTextRegion = first.Region;
    m_FirstToken = first;
    m_LastToken = first;
}

void ParserRule::post_init(const ParserRule& first, const ParserRule& last) noexcept {
    post_init(first.first_token(), last.last_token());
}

void ParserRule::copy_state(const ParserRule& other) noexcept {
    m_TextSource = other.m_TextSource;
    m_FullTextRegion = other.m_FullTextRegion;
    m_FirstToken = other.m_FirstToken;
    m_LastToken = other.m_LastToken;
}

str ParserRule::format_rule() const noexcept {

    if (m_TextSource == nullptr || !m_FullTextRegion) {
        return str{TXT("  > text source is null")};
    }

    constexpr str_view indent{TXT("  > ")};
    str_view text = full_text();
    str_stream out{};
    out << indent;

    // maybe worth adding line numbers into this?
    bool last_was_new_line{false};
    for (str_char elem : text) {
        const bool is_newline = txt::is_newline(elem);
        if (last_was_new_line && is_newline) {
            continue;
        }

        if (is_newline) {
            out << elem << indent;
            last_was_new_line = true;
        } else {
            out << elem;
        }
    }

    return out.str();
}

}  // namespace tm_parse