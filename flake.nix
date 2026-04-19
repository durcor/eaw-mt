{
  description = "EaW engine reverse engineering and multithreading project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    uv2nix.url = "github:pyproject-nix/uv2nix";
    pyproject-nix.url = "github:pyproject-nix/pyproject.nix";
    openspec.url     = "github:Fission-AI/OpenSpec";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        mingw = pkgs.pkgsCross.mingwW64.buildPackages.gcc;
      in {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            # Analysis
            ghidra
            python3
            python3Packages.pefile
            python3Packages.capstone
            radare2

            # Cross-compilation for Windows hook DLLs
            mingw

            # Frida
            frida-tools
            # if outdated in nixpkgs, fall back to getting from pypi
            python3Packages.frida-python

            # Utilities
            git
            file
            xxd
            binutils
          ];

          shellHook = ''
            # Expose analyzeHeadless — it lives in the Ghidra lib/ghidra/support dir
            export PATH="${pkgs.ghidra}/lib/ghidra/support:$PATH"
            echo "eaw-mt dev shell ready"
            echo "MinGW:            $(x86_64-w64-mingw32-gcc --version | head -1)"
            echo "analyzeHeadless:  $(which analyzeHeadless)"
            echo "Frida (Python):   $(python3 -c 'import frida; print(frida.__version__)' 2>/dev/null || echo 'check pip install frida-tools for CLI')"
          '';
        };
      });
}
