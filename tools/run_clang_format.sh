#!/usr/bin/env bash
set -euo pipefail

cd "$(git rev-parse --show-toplevel)"

git ls-files '*.cpp' '*.h' \
  | xargs clang-format -i
