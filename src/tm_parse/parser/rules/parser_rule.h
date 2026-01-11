//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token.h"
#include "tm_parse/parser/matcher.h"
#include "tm_parse/parser/rules/parser_kind.h"
#include "tm_parse/util/text_region.h"

namespace tm_parse {

class Parser;
class ParserRule;
class OutputDevice;
class RuleEvaluator;

// TODO: This is not accurately named this is closer to an AstNode. The static api is closer to
//  what a 'Parser Rule' actually is. But even then the generated result from the Rule::create call
//  is AST like.
class ParserRule {
   public:
    using Ptr = std::unique_ptr<ParserRule>;
    using Vec = std::vector<Ptr>;
    using Iterator = std::vector<Ptr>::iterator;
    using VisitorFunc = std::function<void(const ParserRule&)>;

   protected:
    ParserRule* m_Parent{nullptr};
    const str_char* m_TextSource{nullptr};
    TextRegion m_FullTextRegion;
    Token m_FirstToken;
    Token m_LastToken;

   public:
    ParserRule() = default;
    virtual ~ParserRule();

   public:
    ParserRule(const ParserRule&) = default;
    ParserRule(ParserRule&&) = default;
    ParserRule& operator=(const ParserRule&) = default;
    ParserRule& operator=(ParserRule&&) = default;

   public:
    operator bool() const noexcept { return m_FirstToken; }

   public:
    ParserRule* root() const noexcept;
    ParserRule* parent() const noexcept { return m_Parent; }
    str_view full_text() const;
    const TextRegion& full_text_region() const noexcept { return m_FullTextRegion; }
    const Token& first_token() const noexcept { return m_FirstToken; }
    const Token& last_token() const noexcept { return m_LastToken; }

   public:
    virtual str_view rule_name() const noexcept = 0;
    virtual rkind::ParserRuleKind rule_kind() const noexcept = 0;
    virtual void visit(const VisitorFunc& func) const noexcept { func(*this); }
    void set_parent(ParserRule& parent) noexcept { m_Parent = &parent; }
    int get_depth() const noexcept;

   public:
    void post_init(const Token& first, const Token& last) noexcept;
    void post_init(const Token& first) noexcept;
    void post_init(const ParserRule& first, const ParserRule& last) noexcept;
    void copy_state(const ParserRule& other) noexcept;

   public:
    virtual void cascade_assign_parents(ParserRule* parent) noexcept { m_Parent = parent; };
    // simplifies the child nodes of this node
    virtual void simplify_ast() noexcept {};
    // evaluates the result of this node to produce a numerical value ( if it produces one )
    virtual std::optional<Number> evaluate_numeric_expr() noexcept { return std::nullopt; };

    virtual str format_rule() const noexcept;

   public:
    template <class T>
        requires std::is_base_of_v<ParserRule, T>
    const T* is() const noexcept {
        return (rule_kind() == T::KIND) ? static_cast<const T*>(this) : nullptr;
    }

    template <class T>
        requires std::is_base_of_v<ParserRule, T>
    T* is() noexcept {
        return (rule_kind() == T::KIND) ? static_cast<T*>(this) : nullptr;
    }

    template <class T>
        requires std::is_base_of_v<ParserRule, T>
    const T* as() const {
        if (const T* ptr = is<T>()) {
            return ptr;
        }
        throw std::runtime_error{std::format("can not cast {} to {}", rule_name(), T::NAME)};
    }

    template <class T>
        requires std::is_base_of_v<ParserRule, T>
    T* as() {
        if (T* ptr = is<T>()) {
            return ptr;
        }
        throw std::runtime_error{std::format("can not cast {} to {}", rule_name(), T::NAME)};
    }

    template <class T>
    const T& as_ref() const {
        return *as<T>();
    }

    template <class T>
    T& as_ref() {
        return *as<T>();
    }
};

////////////////////////////////////////////////////////////////////////////////
// | STATIC API GENERATOR |
////////////////////////////////////////////////////////////////////////////////

// TODO: Need to extract this out an put it into the tests section, only need two of the macros here

#define TM_PARSE_TESTS
#ifdef TM_PARSE_TESTS

namespace tests {

struct RuleFactory {
    std::function<bool(Matcher& matcher)> matches;
    std::function<std::unique_ptr<ParserRule>(Parser& matcher)> create;
};

struct RuleTestApi {
    static std::unordered_map<str_view, RuleFactory>& rule_factory() noexcept {
        static std::unordered_map<str_view, RuleFactory> rule_factory{};
        return rule_factory;
    }

    template <class T>
    static void add_rule() noexcept {
        rule_factory()[T::NAME] = RuleFactory{&T::matches, &T::create};
    }

    static const RuleFactory& get_factory(str_view rule) {
        auto& factory = rule_factory();
        auto it = factory.find(rule);
        if (it == factory.end()) {
            throw std::runtime_error{std::format("rule '{}' does not exist", rule)};
        }
        return it->second;
    }
};

// clang-format off
template <class T> struct RuleAutoRegister { RuleAutoRegister() { RuleTestApi::add_rule<T>(); } };
// clang-format on

}  // namespace tests

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TM_PARSE_TEST_API(rule) \
    inline static const ::tm_parse::tests::RuleAutoRegister<rule> RULE_REGISTER {}

#else

#define TM_PARSE_TEST_API(...)

#endif

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RULE_STATIC_CONSTANTS(rule)                                                    \
    constexpr static ::tm_parse::rkind::ParserRuleKind KIND = ::tm_parse::rkind::rule; \
    constexpr static str_view NAME = TXT(#rule);                                       \
    str_view rule_name() const noexcept override {                                     \
        return NAME;                                                                   \
    }                                                                                  \
    ::tm_parse::rkind::ParserRuleKind rule_kind() const noexcept override {            \
        return KIND;                                                                   \
    }

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RULE_STATIC_METHODS(rule)                   \
    static bool matches(Matcher& matcher) noexcept; \
    static std::unique_ptr<rule> create(Parser& parser)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RULE_STATIC_API(rule)    \
    TM_PARSE_TEST_API(rule);     \
    RULE_STATIC_CONSTANTS(rule); \
    RULE_STATIC_METHODS(rule)

}  // namespace tm_parse
