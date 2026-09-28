#!/usr/bin/env nix-shell
#! nix-shell -i bash ./default.nix
set -eu

echo "Prepare directories"
rm -rf ./cover
mkdir -p ./cover/tmp
cd ./cover/tmp

echo "Run gcov"
gcov ../../build/unittest@exe/test_tests.cpp.gcda 1>/dev/null 2>&1
gcov ../../build/unittest@exe/src_intervalset.cpp.gcda 1>/dev/null 2>&1

echo "Only keep interesting files"
cp intervalset.cpp.gcov ../

cd ../..
rm -rf ./cover/tmp

echo "Run gcovr analysis (human-readable report)"
gcovr -gk -o ./cover/summary.txt
cat ./cover/summary.txt

echo "Run gcovr analysis (html report)"
rm -rf ./cover/html
mkdir -p ./cover/html
gcovr -gk --html-details -o ./cover/html/index.html

exit 0
