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

template <class T>
concept is_rule_type_v = std::is_base_of_v<ParserRule, T>;

class ParserRule {
   public:
    using Ptr = std::unique_ptr<ParserRule>;
    using Vec = std::vector<Ptr>;
    using Iterator = std::vector<Ptr>::iterator;

   protected:
    const str_char* m_TextSource;  // Full text source via Parser::text()
    TextRegion m_FullTextRegion;   // Full text region for this rule
    int m_LineNumber;              // Starting line number for this rule

   public:
    ParserRule(const str_char* text_source, const TextRegion& full_text);
    virtual ~ParserRule() = default;

   public:
    const TextRegion& full_text_region() const noexcept { return m_FullTextRegion; }
    str_view full_text() const;

   public:
    virtual str rule_name() const noexcept = 0;
    virtual const Vec* children() const noexcept { return nullptr; }
};

////////////////////////////////////////////////////////////////////////////////
// | PARSER FACTORY |
////////////////////////////////////////////////////////////////////////////////

struct ParseError {
    str Message;       // Failure reason message
    Token FoundToken;  // The token we failed at
};

template <typename RuleType>
struct ParseResult {
    std::unique_ptr<RuleType> Rule;
    std::optional<ParseError> Error;

    bool success() const noexcept { return Rule != nullptr; }

    static ParseResult ok(std::unique_ptr<RuleType> rule) noexcept {
        return ParseResult{std::move(rule), std::nullopt};
    }

    static ParseResult fail(ParseError error) noexcept {
        return ParseResult{nullptr, std::make_optional(std::move(error))};
    }
};

////////////////////////////////////////////////////////////////////////////////
// | STATIC API GENERATOR |
////////////////////////////////////////////////////////////////////////////////

#define RULE_STATIC_API(rule)                     \
    static bool matches(Parser& parser) noexcept; \
    static ParseResult<rule> create(Parser& parser) noexcept;

}  // namespace tm_parse
