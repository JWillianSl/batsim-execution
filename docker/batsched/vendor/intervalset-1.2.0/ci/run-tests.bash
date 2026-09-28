#!/usr/bin/env nix-shell
#! nix-shell -i bash ./default.nix

# Execute the tests
cd build
ninja test
failed=$?

exit ${failed}
