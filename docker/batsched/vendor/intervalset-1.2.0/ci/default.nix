{ doCheck ? false,
  pkgs ? import
    (fetchTarball "https://github.com/NixOS/nixpkgs/tarball/9ac64e5bb65aeca34bef01e48fefaea186428bc1") {},
  kapack ? import
    (fetchTarball "https://github.com/oar-team/kapack/archive/master.tar.gz")
    { inherit pkgs; }
}:

let
  inherit (kapack) intervalset gcovr;
  inherit (kapack.pkgs) nix;
in

(intervalset.override {}).overrideAttrs (attrs: rec {
    name = "intervalset-ci";
    src = ../.;

    preConfigure = ''
      # Remove any existing build directory for a fresh build
      rm -rf ./build/
    '';

    mesonFlags = ["-Db_coverage=true"];

    nativeBuildInputs = attrs.nativeBuildInputs ++ [nix kapack.gcovr];
    inherit doCheck;
})
