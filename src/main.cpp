//
// Date       : 26/08/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"

#include "tm_parse/lexer/lexer.h"

using namespace tm_parse;

int main() {
    // Assumes working directory is: src/test
    fs::path test_file{"./data/simple_lex_test.txt"};

    if (!fs::exists(test_file)) {
        std::cout << "File does not exist" << '\n';
        return 1;
    }

    using It = std::istreambuf_iterator<char>;
    std::ifstream test_stream{test_file};
    std::string test_str{It{test_stream}, It{}};

    Lexer lexer{test_str};
    Token tk{};

    do {
        tk = lexer.next_token();
        if (tk.Kind == tk::TokenKind::EndOfInput) {
            std::cout << "EOF" << '\n';
        } else {
            std::cout << std::format(
                "[{:>2}] ({:>2}, {:>2}) -> '{}'",
                lexer.current_line(),
                tk.Region.Start,
                tk.Region.End,
                tk.Region.create_str(test_str)
            ) << '\n';
        }
    } while (tk != tk::TokenKind::EndOfInput);
}