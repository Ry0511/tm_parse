//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/parser/parser_rule.h"

namespace tm_parse {

class Parser;

struct ParseError {
    str Message;       // Failure reason message
    Token FoundToken;  // The token we failed at
};

template <class RuleType>
    requires(std::is_base_of_v<ParserRule, RuleType>)
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

#define RULE_FACTORY_STUBS(rule)                  \
    using ResultType = ParseResult<rule>;         \
    static bool matches(Parser& parser) noexcept; \
    static ResultType create(Parser& parser) noexcept;

}  // namespace tm_parse