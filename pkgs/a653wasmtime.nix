{ lib, stdenv, bintools, c-abi-lens, wasmtime }:

stdenv.mkDerivation {
  pname = "a653-wasmtime";
  version = "unstable-2024-04-17";

  src = ./..;

  patches = [ ../patches/allow-cc-override.patch ];
  preConfigure = ''
    mkdir home
    export HOME="$PWD/home"
  '';

  buildInputs = [ bintools wasmtime.dev ];

  preBuild = ''
    mkdir -p $PWD/home/tmp/arinc653-wasm/pkgs/c-abi-lens/target/debug
    # Use the pre-built c-abi-lens from the flake input
    ln -s ${c-abi-lens}/bin/c-abi-lens $PWD/home/tmp/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens
  '';

  makeFlags = [
    "CC=${stdenv.cc.targetPrefix}gcc"
    "AR=${stdenv.cc.targetPrefix}ar"
    "RANLIB=${stdenv.cc.targetPrefix}ranlib"
  ];

  buildFlags = [ "wasm_host" ];

  installPhase = ''
    mkdir --parent -- $out/bin
    mv $HOME/bin/* $out/bin
  '';
  dontStrip = true;
}
