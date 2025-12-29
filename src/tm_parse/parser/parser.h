//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/lexer/lexer.h"
#include "tm_parse/parser/matcher.h"
#include "tm_parse/parser/rules/parser_rule.h"

namespace tm_parse {

// TODO: Need to revisit this Parser/Matcher stuff - ideally we would want context on the parse tree
//  when it fails. Currently we have no way of knowing where it got to. This context would be
//  required for informing of errors and potential fixes.

struct ParseStateInfo {
    bool AllowUnquotedStrings{true};
    bool AllowCreateMod{true};
};

class Parser : public Matcher {
   private:
    str m_Text;
    std::vector<Token> m_Tokens;
    ParseStateInfo m_ParseStateInfo;

   public:
    explicit Parser(str text);
    ~Parser() = default;

   public:
    std::unique_ptr<ParserRule> parse();

   public:
    str_view text() const noexcept { return m_Text; }
    ParseStateInfo& parse_state() noexcept { return m_ParseStateInfo; }

   public:
    Matcher create_matcher() noexcept { return Matcher{*this}; }

   public:
    template <class T>
        requires std::is_base_of_v<ParserRule, T>
    std::unique_ptr<T> create() {
        return T::create(*this);
    }

    template <class T, class... Tail>
        requires std::is_base_of_v<ParserRule, T>
    std::unique_ptr<ParserRule> create_one_of() {
        size_t pos = position();
        if (matches<T>()) {
            set_position(pos);
            return create<T>();
        }

        if constexpr (sizeof...(Tail) == 0) {
            throw std::runtime_error{std::format("{}", get_error_string("something"))};
        } else {
            return create_one_of<Tail...>();
        }
    }

   private:
    friend class Matcher;
};

}  // namespace tm_parse