//
// Date       : 28/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "dot_identifier.h"
#include "tm_parse/parser/parser.h"
#include "tm_parse/parser/rules/object_ref.h"

namespace tm_parse::rules {

bool ObjectRef::matches(Parser& parser) noexcept {
    return DotIdentifier::matches(parser);
}

std::unique_ptr<ObjectRef> ObjectRef::create(Parser& parser) {
    ObjectRef ref{};

    ref.MainObject = DotIdentifier::create(parser);

    if (parser.maybe(tk::Colon)) {
        ref.SubObject = DotIdentifier::create(parser);
        ref.post_init(ref.MainObject->first_token(), ref.SubObject->last_token());
    } else {
        ref.copy_state(*ref.MainObject);
    }

    return std::make_unique<ObjectRef>(std::move(ref));
}

}  // namespace tm_parse::rules