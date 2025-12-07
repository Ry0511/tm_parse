//
// Date       : 11/09/2025
// Project    : tm_parse
// Author     : -Ry
//

#include "tm_parse/pch.h"
#include "tm_parse/util/text_helpers.h"
#include "test_file.h"
#include "test_runner.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

namespace tm_parse::tests {

namespace {

int failure_count = 0;
int success_count = 0;
bool log_everything = false;

bool has_parent(const fs::path& file, std::string_view name) {
    fs::path cur = file;
    while (cur.has_parent_path()) {
        if (cur == cur.parent_path()) {
            return false;
        }

        cur = cur.parent_path();
        if (cur.filename().string() == name) {
            return true;
        }
    }
    return false;
}

void run_test(const fs::path& test_file) {
    TestFile test{test_file};

    const str& test_type = test.test_type();
    auto runner = test.create_test_runner();

    if (runner->run(test)) {
        LOG_INFO("[ \033[32m{}\033[0m ] - {}", "TEST PASSED", test_file.filename().string());
        ++success_count;
    } else {
        LOG_INFO("[ \033[31m{}\033[0m ] - {}", "TEST FAILED", test_file.filename().string());
        ++failure_count;
    }

    if (log_everything || !runner->success()) {
        for (const str& msg : runner->execution_log()) {
            LOG_INFO("{}", msg);
        }
    }
}

void run_all_tests(const fs::path& directory) {
    for (const auto& entry : fs::recursive_directory_iterator{directory}) {
        if (entry.is_regular_file()) {
            try {
                const auto& file = entry.path();
                std::string filename = file.extension().string();
                if (txt::equal_icase(filename, ".ltest") || txt::equal_icase(filename, ".ptest")) {
                    run_test(entry.path());
                } else if (has_parent(file, "examples")) {
                    str_ifstream ss{file};
                    Parser parser{
                        str{str_istreambuf_it{ss}, str_istreambuf_it{}}
                    };
                    auto rule = parser.parse();
                    LOG_INFO("Successfully parsed example file: {}", file.filename().string());
                }
            } catch (const std::exception& err) {
                LOG_INFO("Error running test {}", entry.path().string());
                LOG_INFO("With message: {}", err.what());
            }
        }
    }
}

}  // namespace

}  // namespace tm_parse::tests

int main() {
    using namespace tm_parse;

    LOG_INFO("Running tm_parse tests...");
    tests::run_all_tests(fs::current_path() / "data");

    LOG_INFO("Running all catch2 tests...");
    Catch::Session session{};
    int res = session.run();

    if (res != 0) {
        LOG_ERR("Some catch2 tests failed...");
        return res;
    }

    return tests::failure_count;
}