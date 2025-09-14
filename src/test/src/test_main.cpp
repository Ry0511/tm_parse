//
// Date       : 11/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "test_file.h"
#include "test_runner.h"

namespace tm_parse::tests {

namespace {

int failure_count = 0;
int success_count = 0;
bool log_everything = false;

void run_test(const fs::path& test_file) {
    TestFile test{test_file};

    const str& test_type = test.test_type();

    auto runner = test.create_test_runner();
    if (runner->run(test)) {
        INFO("[ \033[32m{}\033[0m ] - {}", "TEST PASSED", test_file.filename().string());
        ++success_count;
    } else {
        INFO("[ \033[31m{}\033[0m ] - {}", "TEST FAILED", test_file.filename().string());
        ++failure_count;
    }

    if (log_everything || !runner->success()) {
        for (const str& msg : runner->execution_log()) {
            INFO("{}", msg);
        }
    }
}

void run_all_tests(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator{directory}) {
        if (entry.is_regular_file()) {
            try {
                run_test(entry.path());
            } catch (const std::exception& err) {
                INFO("Error running test {}", entry.path().string());
                INFO("With message: {}", err.what());
            }
        }
    }
}

}  // namespace

}  // namespace tm_parse::tests

int main() {
    using namespace tm_parse;
    tests::run_all_tests(fs::current_path() / "data");

    TRACE("Hello World");
    INFO("Hello World");
    WARN("Hello World");
    ERR("Hello World");

    return tests::failure_count;
}