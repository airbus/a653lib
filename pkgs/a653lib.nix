{
  stdenv,
  cmake,
}:

stdenv.mkDerivation {
  pname = "a653lib";
  version = "unstable-2025-11-26";

  src = ./..;

  nativeBuildInputs = [ cmake ];

  dontStrip = true;

  # TODO investigate why hardening causes crashes
  hardeningDisable = [ "all" ];
}
