#!/usr/bin/env nix-shell
#! nix-shell -i bash ./default.nix
set -eux

# Start from a clean build directory
rm -rf ./build

# Usual meson build stuff
meson build -Db_coverage=true
cd build
ninja
