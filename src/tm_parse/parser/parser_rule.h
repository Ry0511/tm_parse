//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token.h"
#include "tm_parse/util/text_region.h"

namespace tm_parse {

class Parser;
class ParserRule;

class ParserRule {
   public:
    using Ptr = std::unique_ptr<ParserRule>;
    using Vec = std::vector<Ptr>;
    using Iterator = std::vector<Ptr>::iterator;

   protected:
    const str_char* m_TextSource{nullptr};
    TextRegion m_FullTextRegion;
    Token m_FirstToken;
    Token m_LastToken;

   public:
    ParserRule() = default;
    virtual ~ParserRule() = default;

   public:
    ParserRule(const ParserRule&) = default;
    ParserRule(ParserRule&&) = default;
    ParserRule& operator=(const ParserRule&) = default;
    ParserRule& operator=(ParserRule&&) = default;

   public:
    operator bool() const noexcept { return m_FirstToken; }

   public:
    const TextRegion& full_text_region() const noexcept { return m_FullTextRegion; }
    str_view full_text() const;
    const Token& first_token() const noexcept { return m_FirstToken; }
    const Token& last_token() const noexcept { return m_LastToken; }

   public:
    virtual str rule_name() const noexcept = 0;
    virtual const Vec* children() const noexcept { return nullptr; }

   protected:
    void post_init(const Token& first, const Token& last) noexcept;
    void copy_state(const ParserRule& other) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
// | STATIC API GENERATOR |
////////////////////////////////////////////////////////////////////////////////

#define RULE_STATIC_API(rule)                     \
    static bool matches(Parser& parser) noexcept; \
    static std::unique_ptr<rule> create(Parser& parser);

}  // namespace tm_parse
