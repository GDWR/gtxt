{
  description = "The text editor that couldn't";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
        let pkgs = nixpkgs.legacyPackages.${system}; in {
            packages = rec {
              gtxt = pkgs.stdenv.mkDerivation {
                  name = "gtxt";
                  src = ./.;
                  nativeBuildInputs = with pkgs; [
                      cmake
                      freetype
                      SDL2
                  ];
                  installPhase = ''
                    mkdir -p $out/bin
                    cp gtxt $out/bin
                  '';
              };
              default = gtxt;
            };
          }
    );

}
