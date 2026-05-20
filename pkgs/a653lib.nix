{
  lib,
  stdenv,
  cmake,
  fetchurl,
  gnumake,
  pkg-config,
  clang ? null,
  wasiSdk ? null,
  wasmtime ? null,
  wamr ? null,
  cAbiLens ? null,
  enableWasm ? false
}:

assert !enableWasm || clang != null;
assert !enableWasm || wasiSdk != null;
assert !enableWasm || wasmtime != null;
assert !enableWasm || wamr != null;
assert !enableWasm || cAbiLens != null;

let
  arinc653Zip = fetchurl {
    url = "https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip";
    sha256 = "1xml8ki31pidxp5gzyqb4bp819809zpx4kkg2k5x1nhar50gxjp2";
    curlOptsList = [
      "--user-agent"
      "Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0"
    ];
  };
in
stdenv.mkDerivation {
  pname = if enableWasm then "a653lib-wasm" else "a653lib";
  version = "unstable-2025-11-26";
  src = ./..;

  nativeBuildInputs =
    [ cmake gnumake pkg-config ]
    ++ lib.optionals enableWasm [ clang ];

  buildInputs =
    lib.optionals enableWasm [ wasmtime wamr ];

  cmakeFlags =
    [
      "-DARINC653_ZIP=${arinc653Zip}"
    ]
    ++ lib.optionals enableWasm [
      "-DA653LIB_BUILD_WASM=ON"
      "-DA653LIB_FETCH_C_ABI_LENS=OFF"
      "-DWASM_CLANG=${lib.getExe clang}"
      "-DWASI_SYSROOT=${wasiSdk}/share/wasi-sysroot"
      "-DC_ABI_LENS_EXECUTABLE=${lib.getExe cAbiLens}"
    ];

  dontStrip = true;

  hardeningDisable = [ "all" ];
}
