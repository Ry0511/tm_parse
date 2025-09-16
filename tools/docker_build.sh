#!/usr/bin/env bash

BUILD_TYPE=${BUILD_TYPE:-debug}
ARCH=${ARCH:-x86}
CLEAN=${CLEAN:-false}
BUILD_PRESET="clang-$BUILD_TYPE-$ARCH"

cd tm_parse

if [[ "$CLEAN" == "true" ]]; then
  rm -fr "out/build/$BUILD_PRESET"
fi

cmake . --preset "$BUILD_PRESET" -G Ninja
cmake --build "out/build/$BUILD_PRESET"

cd src/test
../../out/build/clang-debug-x86/tm_parse_tests

cd /git_repos