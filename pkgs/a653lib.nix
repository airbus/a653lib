{ lib, stdenv, fetchFromGitHub, bintools }:

stdenv.mkDerivation {
  pname = "a653lib";
  version = "unstable-2024-04-17";

  src = fetchFromGitHub {
    owner = "airbus";
    repo = "a653lib";
    rev = "75d7644728da420e6b640cb5b821ab0a2b1a3fe2";
    hash = "sha256-Q/LeKqU+wql9utqwbPvrzAm9o1gjlGK2NunKWr0S73c=";
  };

  patches = [ ../patches/allow-cc-override.patch ];
  preConfigure = ''
    mkdir home
    export HOME="$PWD/home"
  '';

  buildInputs = [ bintools ];

  makeFlags = [
    "CC=${stdenv.cc.targetPrefix}gcc"
    "AR=${stdenv.cc.targetPrefix}ar"
    "RANLIB=${stdenv.cc.targetPrefix}ranlib"
  ];

  installPhase = ''
    mkdir --parent -- $out/bin
    mv $HOME/bin/* $out/bin
  '';
  dontStrip = true;

  # TODO investigate why hardening causes crashes
  hardeningDisable = [ "all" ];
}
