{
  description = "An ARINC 653 implementation on top of POSIX";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.arinc653-wasm.url = "github:psiegl/arinc653-wasm/psiegl-old";

  outputs = { self, nixpkgs, flake-utils, arinc653-wasm }:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" "powerpc64-linux"] (system:
      let
        # import the nixpkgs, passing the current system as arg
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      {
        packages = rec {
          # define default package
          default = a653lib;

          # just call the package with the normal stdenv
          a653lib = pkgs.callPackage pkgs/a653lib.nix { };

          # Add needed build tools via overrideAttrs (do NOT pass nativeBuildInputs to callPackage)
          arinc653Packages = arinc653-wasm.packages.${system} or {};

          a653-wasmtime = pkgs.callPackage pkgs/a653wasmtime.nix {
            inherit (arinc653Packages) c-abi-lens;
          };

          # override the stdenv to use an older gcc
          a653lib-gcc11 =
            let
              stdenv = pkgs.overrideCC pkgs.stdenv pkgs.gcc11;
            in
            pkgs.callPackage pkgs/a653lib.nix { inherit stdenv; };

          # override the build to be with debug symbols (source still missing though)
          a653lib-debug = pkgs.enableDebugging a653lib;

          # cross compile for a pre-defined target platform
          a653lib-aarch64 = pkgs.pkgsCross.aarch64-multiplatform-musl.pkgsStatic.callPackage pkgs/a653lib.nix { };

         # cross compile for a custom defined target platform
          a653lib-armv7a =
            let
              # import nixpkgs, defining a crossSystem via llvm target triple
              pkgsCross = import nixpkgs {
                inherit system;
                crossSystem.config = "armv7l-unknown-linux-musleabi";
              };
            in
            pkgsCross.callPackage pkgs/a653lib.nix { };

          # cross compile for a custom defined target platform
          a653lib-powerpc64 =
            let
              # import nixpkgs, defining a crossSystem via llvm target triple
              pkgsCross = import nixpkgs {
                inherit system;
                crossSystem.config = "powerpc64-unknown-linux-gnuabielfv2";
              };
            in
            pkgsCross.pkgsStatic.callPackage pkgs/a653lib.nix { };
        };
      }
    );
}
