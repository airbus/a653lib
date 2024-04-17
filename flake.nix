{
  description = "An ARINC 653 implementation on top of POSIX";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem [ "x86_64-linux" "aarch64-linux" ] (system:
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
        };
      }
    );
}
