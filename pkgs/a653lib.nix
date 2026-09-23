{
  lib,
  stdenv,
  fetchurl,
  pkgsBuildBuild,

  cmake,
  pkg-config,
  pkgsCross,

  # these are only required when Wasm is enabled
  wamr,
  wasmtime,

  arinc653HeaderZip ? fetchurl {
    url = "https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip";
    hash = "sha256-4sr+QMkK2tDLFG9O0u9PAKWA7iIL+//K7S3eMOJEtPY=";
  },

  enableWasm ? (with stdenv.hostPlatform; (!isStatic) && (isx86 || isAarch64)),
}:

stdenv.mkDerivation {
  name = "a653lib";
  src = ./..;

  nativeBuildInputs = [
    cmake
    pkg-config
  ]
  ++ (lib.lists.optional enableWasm pkgsCross.wasi32.stdenv.cc);

  postPatch = lib.strings.optionalString enableWasm ''
    substituteInPlace  CMakeLists.txt \
      --replace-fail 'wasm32-wasip1' 'wasm32-unknown-wasi'
  '';

  buildInputs = lib.lists.optionals enableWasm [
    # Avoid the following error upon linking a653lib
    #
    # undefined references to `__aarch64_ldadd4_acq'
    (wamr.overrideAttrs (_: {
      env = lib.attrsets.optionalAttrs (stdenv.hostPlatform.isAarch) {
        NIX_CFLAGS_COMPILE = "-mno-outline-atomics";
      };
    }))
    wasmtime
  ];

  cmakeFlags = [
    "-DARINC653_ZIP=${arinc653HeaderZip}"
    "-DA653LIB_BUILD_WASM=off"
  ]
  ++ lib.lists.optionals enableWasm [
    "-DA653LIB_BUILD_WASM=on"

    "-DA653LIB_FETCH_C_ABI_LENS=off"
    "-DC_ABI_LENS_EXECUTABLE=${lib.meta.getExe pkgsBuildBuild.c-abi-lens}"

    "-DWASM_CLANG=${lib.meta.getExe' pkgsCross.wasi32.stdenv.cc "${pkgsCross.wasi32.stdenv.cc.targetPrefix}cc"}"
  ];

  makeFlags = lib.lists.optionals enableWasm [
    "partition_a_wasm"
    "partition_b_wasm"
  ];

  postInstall = lib.strings.optionalString enableWasm ''
    for engine in wamr wasmtime
    do
      target_dir="$out/bin/with-$engine"
      mkdir --parent -- "$target_dir"
      ln --relative --symbolic --force -- $out/bin/*wasm "$target_dir"
      ln --relative --symbolic --force -- "$out/bin/p_$engine" "$target_dir/wasm32_rt"
    done
  '';

  meta = {
    description = "AIRBUS' Linux based ARINC 653 playground, now with Wasm";
    license = lib.licenses.lgpl21Plus;
    maintainers = with lib.maintainers; [ wucke13 ];
    mainProgram = "a653lib_main";
    platforms = lib.platforms.linux;
  };
}
