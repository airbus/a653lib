{ lib, stdenv, fetchurl, fetchFromGitHub, bintools }:

let
  arinc653Zip = fetchurl {
    url = "https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip";
    sha256 = "1xml8ki31pidxp5gzyqb4bp819809zpx4kkg2k5x1nhar50gxjp2";
    curlOptsList = [ "--user-agent" "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0" ];
  };
in

stdenv.mkDerivation {
  pname = "a653lib";
  version = "unstable-2024-04-17";

  src = ./..;

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

    cp ${arinc653Zip}/ARINC653.h $(HOME)/tmp/download/ARINC653.h
  '';
  dontStrip = true;

  # TODO investigate why hardening causes crashes
  hardeningDisable = [ "all" ];
}
