{ lib, stdenv, fetchzip, bintools, gawk }:

let
  arinc653Zip = fetchzip {
    url = "https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip";
    sha256 = "sha256-BGR3haRc+5I9VuyI+P8E/qTNnqVhnJKjP7j7RRCMFwg=";
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

    # make the Makefile happy
    mkdir --parent -- $HOME/tmp/download
    touch $HOME/tmp/download/arinc653.h.zip
    cp ${arinc653Zip}/ARINC653.h $HOME/tmp/download/.
  '';

  buildInputs = [ bintools gawk ];

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
