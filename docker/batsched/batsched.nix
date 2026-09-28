{ nurKapackRev, batschedSrcRev }:
let
  kapack = import (fetchTarball
    "https://github.com/oar-team/nur-kapack/archive/${nurKapackRev}.tar.gz") {};
  lib = kapack.pkgs.lib;
  myIntervalset = kapack.intervalsetlight.overrideAttrs (old: {
    src = ./vendor/intervalset-1.2.0;
  });
  replaceIntervalset = deps:
    assert lib.any (d: (d.name or "") == "intervalset-1.2.0") deps;
    map (d: if (d.name or "") == "intervalset-1.2.0" then myIntervalset else d) deps;
in
kapack.batsched.overrideAttrs (old: {
  src = kapack.pkgs.fetchFromGitHub {
    owner = "oar-team";
    repo = "batsched";
    rev = batschedSrcRev;
    sha256 = "12r8b14rwa26wx34l1492vdvyn2s7mch3ixlz46s6imi1ximywa8";
  };
  buildInputs = replaceIntervalset old.buildInputs;
})
