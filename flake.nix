{
  description = "An ARINC 653 implementation on top of POSIX";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachSystem ["x86_64-linux" "aarch64-linux" "powerpc64-linux"] (system:
      let
        # import the nixpkgs, passing the current system as arg
        pkgs = import nixpkgs {
          inherit system;
        };

        # Add needed build tools via overrideAttrs (do NOT pass nativeBuildInputs to callPackage)
        addBuildDeps = pkg: pkg.overrideAttrs (oldAttrs: {
          nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [ pkgs.git pkgs.gcc pkgs.gnu_make ];
          buildInputs = (oldAttrs.buildInputs or []) ++ [ pkgs.git ];
        });
      in
      {
        packages = rec {
          # just call the package with the normal stdenv
          a653lib = pkgs.callPackage pkgs/a653lib.nix { };

          # Apply overrideAttrs here, not an argument to callPackage
          a653libWithDeps = addBuildDeps a653lib;

          default = a653libWithDeps;

          # override the stdenv to use an older gcc
          a653lib-gcc11 =
            let
              stdenv = pkgs.overrideCC pkgs.stdenv pkgs.gcc11;
              base = pkgs.callPackage pkgs/a653lib.nix { inherit stdenv; };
            in
            base.overrideAttrs (oldAttrs: {
              nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [ pkgs.git pkgs.gcc pkgs.gnumake ];
              buildInputs = (oldAttrs.buildInputs or []) ++ [ pkgs.git ];
            });

          # override the build to be with debug symbols (source still missing though)
          a653lib-debug = pkgs.enableDebugging a653libWithDeps;

          # cross compile for a pre-defined target platform
          a653lib-aarch64 =
            let
              pkgsCross = pkgs.pkgsCross.aarch64-multiplatform-musl.pkgsStatic;
              base = pkgsCross.callPackage pkgs/a653lib.nix { };
            in
            base.overrideAttrs (oldAttrs: {
              nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [ pkgsCross.git pkgsCross.gcc pkgsCross.gnumake ];
              buildInputs = (oldAttrs.buildInputs or []) ++ [ pkgsCross.git ];
            });

         # cross compile for a custom defined target platform
          a653lib-armv7a =
            let
              # import nixpkgs, defining a crossSystem via llvm target triple
              pkgsCross = import nixpkgs {
                inherit system;
                crossSystem.config = "armv7l-unknown-linux-musleabi";
              };
              base = pkgsCross.callPackage pkgs/a653lib.nix { };
            in
            base.overrideAttrs (oldAttrs: {
              nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [ pkgsCross.git pkgsCross.gcc pkgsCross.gnumake ];
              buildInputs = (oldAttrs.buildInputs or []) ++ [ pkgsCross.git ];
            });

          # cross compile for a custom defined target platform
          a653lib-powerpc64 =
            let
              # import nixpkgs, defining a crossSystem via llvm target triple
              pkgsCross = import nixpkgs {
                inherit system;
                crossSystem.config = "powerpc64-unknown-linux-gnuabielfv2";
              };
              base = pkgsCross.pkgsStatic.callPackage pkgs/a653lib.nix { };
            in
            base.overrideAttrs (oldAttrs: {
              nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [ pkgsCross.git pkgsCross.gcc pkgsCross.gnumake ];
              buildInputs = (oldAttrs.buildInputs or []) ++ [ pkgsCross.git ];
            });
        };
      }
    );
}
