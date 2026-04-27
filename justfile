wine    := "/nix/store/9rcmpchnqdivam5i2fpzjh27mlq8m4px-proton-ge-bin-GE-Proton10-34-steamcompattool/files/bin/wine64"
proton  := "/nix/store/9rcmpchnqdivam5i2fpzjh27mlq8m4px-proton-ge-bin-GE-Proton10-34-steamcompattool/proton"
game-dir  := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'/corruption"
game-base := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'"
compat-data := env('HOME') + "/gam/steam/steamapps/compatdata/32470"
log-file  := env('HOME') + "/gam/steam/steamapps/common/'Star Wars Empire at War'/corruption/eaw-mt.log"

export WINEPREFIX := env('HOME') + "/gam/steam/steamapps/compatdata/32470/pfx"

build-winmm:
  nix develop --command x86_64-w64-mingw32-gcc -shared -o patches/experimental/winmm.dll hooks/winmm_proxy.c -lkernel32
  cp patches/experimental/winmm.dll {{game-dir}}

ghidra-script script:
  nix develop --command /nix/store/qq2mn7zc8f3q04nb4s538ykffqk3qbq4-ghidra-12.0.4/lib/ghidra/support/analyzeHeadless \
    ghidra_projects EawProject \
    -process StarWarsG.exe \
    -noanalysis \
    -scriptPath tools/ghidra_scripts \
    -postScript {{script}}.java \
    -log logs/{{script}}.log

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

# Launch via Steam URI — launches the EaW base game, NOT Forces of Corruption
# and NOT the mod configured in launch options. Use the Steam UI Play button
# with the correct launch option selected instead.
run-game:
  steam "steam://rungameid/32470"

# Wait for StarWarsG.exe to appear (up to 60s), then print its PID.
wait-game:
  timeout 60 sh -c 'until pgrep -x StarWarsG.exe > /dev/null; do sleep 1; done && pgrep -x StarWarsG.exe'

# Kill a running game instance.
kill-game:
  pkill -x StarWarsG.exe || true
