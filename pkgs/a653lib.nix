{
  stdenv,
  bintools,
}:

stdenv.mkDerivation {
  pname = "a653lib";
  version = "unstable-2024-04-17";

  src = ./..;

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
