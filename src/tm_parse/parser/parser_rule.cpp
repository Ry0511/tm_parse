//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser_rule.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse {

ParserRule::ParserRule(const str_char* text_source, const TextRegion& full_text)
    : m_TextSource(text_source),
      m_FullTextRegion(full_text),
      m_LineNumber(0) {} // TODO: Fix

str_view ParserRule::full_text() const {
    if (m_TextSource == nullptr) {
        throw std::runtime_error{"text source is null"};
    }

    return m_FullTextRegion.create_str_view(m_TextSource);
}

}  // namespace tm_parse