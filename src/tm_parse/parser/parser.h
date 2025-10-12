//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"
#include "rules/parser_rule.h"
#include "tm_parse/lexer/lexer.h"
#include "tm_parse/parser/matcher.h"

namespace tm_parse {

class Parser {
   private:
    str m_Text;
    std::vector<Token> m_Tokens;
    Matcher m_Matcher;

   public:
    Parser(str text);
    Parser(const fs::path& file);
    ~Parser() = default;

   public:
    bool is_eof() const { return m_Matcher.is_eof(); }

   public:
    std::unique_ptr<ParserRule> parse();

   public:
    str_view text() const noexcept { return m_Text; }
    size_t position() const noexcept { return m_Matcher.position(); }
    void set_position(size_t pos) noexcept { m_Matcher.set_position(pos); }

   public:
    Matcher create_matcher() const noexcept { return Matcher{*this}; }

    // TODO: replace this by inheriting Matcher. Only delaying this since the above conversion
    //  operator and some other places make a bunch of rules fail to parse. Once proper testing is
    //  being done it will be easier to figure out what will get broken.
   public: // clang-format off
    Token next()                                         noexcept { return m_Matcher.next();             }
    Token next_real()                                    noexcept { return m_Matcher.next_real();        }
    Token maybe(tk::TokenKind kind)                      noexcept { return m_Matcher.maybe(kind);        }
    Token maybe_real(tk::TokenKind kind)                 noexcept { return m_Matcher.maybe_real(kind);   }
    Token any(std::span<const tk::TokenKind> kinds)      noexcept { return m_Matcher.any(kinds);         }
    Token any_real(std::span<const tk::TokenKind> kinds) noexcept { return m_Matcher.any_real(kinds);    }
    Token require(tk::TokenKind kind)                             { return m_Matcher.require(kind);      }
    Token require_real(tk::TokenKind kind)                        { return m_Matcher.require_real(kind); }
    // clang-format on

   private:
    friend class Matcher;
};

}  // namespace tm_parse