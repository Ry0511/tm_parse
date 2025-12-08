//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse {

str_view ParserRule::full_text() const {
    if (m_TextSource == nullptr) {
        throw std::runtime_error{"text source is null"};
    }

    return full_text_region().create_str_view(m_TextSource);
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

ParserRule::~ParserRule() = default;

}  // namespace tm_parse