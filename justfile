wine := "/nix/store/9rcmpchnqdivam5i2fpzjh27mlq8m4px-proton-ge-bin-GE-Proton10-34-steamcompattool/files/bin/wine64"
game-dir := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'/corruption"

export WINEPREFIX := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx"

ghidra-script script:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis \
    -scriptPath tools/ghidra_scripts \
    -postScript {{script}}.java \
    -log logs/{{script}}.log

build-winmm:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

build-hook:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/eaw-mt-hook.dll hooks/eaw-mt-hook.c -lkernel32

  cp patches/experimental/eaw-mt-hook.dll {{game-dir}}
  cp patches/experimental/eaw-mt-hook.dll $WINEPREFIX/drive_c/windows/system32

  {{wine}} reg add \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v AppInit_DLLs /t REG_SZ /d "eaw-mt-hook.dll" /f
  {{wine}} reg add \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v LoadAppInit_DLLs /t REG_DWORD /d 1 /f

remove-hook:
  {{wine}} reg delete \
    "HKLM\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows" \
    /v AppInit_DLLs /f

run-game:
  WINEDLLOVERRIDES="winmm=n,b" WINEDEBUG=+debugstr PROTON_USE_NTSYNC=1 %command% STEAMMOD=1125571106
