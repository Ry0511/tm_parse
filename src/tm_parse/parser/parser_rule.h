//
// Date       : 21/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#pragma once

#include "tm_parse/pch.h"

#include "tm_parse/lexer/token.h"
#include "tm_parse/parser/rule_kind.h"
#include "tm_parse/util/text_region.h"

namespace tm_parse {

class Parser;

struct SourceInfo {
    int StartLineNumber;
    int EndLineNumber;
    str_view RuleSourceText;
};

class ParserRule {
   public:
    using Ptr = std::unique_ptr<ParserRule>;
    using Vec = std::vector<Ptr>;
    using Iterator = std::vector<Ptr>::iterator;

   protected:
    Parser* m_Parser;
    TextRegion m_FullTextRegion;
    SourceInfo m_SourceInfo;
    rules::RuleKind m_RuleKind;

   public:
    ParserRule() = default;
    virtual ~ParserRule() = default;

   public:
    const Parser& parser() const noexcept { return *m_Parser; }
    const TextRegion& full_text_region() const noexcept { return m_FullTextRegion; }
    const SourceInfo& source_info() const noexcept { return m_SourceInfo; }
    const rules::RuleKind rule_kind() const noexcept { return m_RuleKind; }

    str_view full_text() const;

   public:
    virtual str rule_name() const noexcept = 0;
    virtual const Vec* const children() const noexcept { return nullptr; }

   public:  // clang-format off
    template <class T> const T& as() const { return dynamic_cast<const T&>(*this); }
    template <class T> T& as() { return dynamic_cast<T&>(*this); }
    // clang-format on
};

}  // namespace tm_parse
