//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser_rule.h"
#include "tm_parse/parser/parser.h"

namespace tm_parse {

str_view ParserRule::full_text() const {
    return m_FullTextRegion.create_str_view(m_Parser->text());
}

}  // namespace tm_parse